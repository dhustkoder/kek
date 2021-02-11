#include "kek.h"
#include <assert.h>
#include <limits.h>

#define QNODE_CAPACITY 4

static MemPool pool;


static QNode *take_qnode_leaf(QTree *tree, int x, int y, int width, int height, size_t depth);
static QNode *take_qnode_data(QTree *tree, int x, int y, void *data);
static bool qnode_in_region(QNode *node, int x, int y);
static bool qnode_insert(QNode *parent, QNode *leaf);
static void qnode_subdivide(QNode *parent);
static bool qnode_detach_data(QNode *node);
static QNode *qtree_insert_data(QNode *data_node);
static void qnode_query(QNode *node, int x0, int y0, int x1, int y1, QTreeQueryFn fn, void *ctx);

void qtree_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(QTree));
}

QTree *qtree_create(MemPool *qnode_pool, size_t max_depth)
{
    QTree *tree = mem_pool_take(&pool);

    tree->node_pool = qnode_pool;
    tree->root = NULL;
    tree->max_depth = max_depth;


    return tree;
}

static void print_indent(int depth)
{
    for(int i = 0; i < depth; ++i) 
        printf("    ");
}


void qtree_print(QNode *node)
{
    if(node->type == QNODE_PARENT || node->type == QNODE_LEAF)
    {
        print_indent(node->depth);
        printf("#%p: (%d,%d)[%d,%d]\n", node, node->x, node->y, node->width, node->height);
    }
    if(node->type == QNODE_LEAF)
    {
        QNode *child = node->children;
        while(child)
        {
            print_indent(node->depth + 1);
            printf("->%p: (%d,%d) %p\n", child, child->x, child->y, child->data);
            child = child->next;
        }
    }
    if(node->type == QNODE_PARENT)
    {
        QNode *child = node->children;
        while(child)
        {
            qtree_print(child);
            child = child->next;
        }
    }

}

void qtree_query(QTree *tree, int x0, int y0, int x1, int y1, QTreeQueryFn fn, void *ctx)
{
    if(!tree->root)
        return;

    qnode_query(tree->root, x0, y0, x1, y1, fn, ctx);
}


static void qnode_query(QNode *node, int x0, int y0, int x1, int y1, QTreeQueryFn fn, void *ctx)
{

    int bx0 = node->x;
    int bx1 = node->x + node->width;

    int by0 = node->y;
    int by1 = node->y + node->height;

    // AABB range check
	bool range = (x0 < bx1 && x1 > bx0 && y0 < by1 && y1 > by0);

    if(!range)
        return;

    if(node->type == QNODE_PARENT)
    {
        QNode *child = node->children;

        while(child)
        {
            qnode_query(child, x0, y0, x1, y1, fn, ctx);
            child = child->next;
        }
    }
    if(node->type == QNODE_LEAF)
    {
        QNode *child = node->children;

        while(child)
        {
            fn(child, ctx);
            child = child->next;
        }
    }
}


void qtree_destroy_node(QNode *node)
{
    qnode_detach_data(node);

    mem_pool_release(node->tree->node_pool, node);
}

QNode *qtree_move_node(QNode *node, int x, int y)
{
    assert(node->type == QNODE_DATA);
    QNode *parent = node->parent;

    qnode_detach_data(node);
    node->x = x;
    node->y = y;

    qtree_insert_data(node);
}

QNode *qtree_create_node(QTree *tree, void *data)
{
    return take_qnode_data(tree, 0, 0, data);
}

QNode *qtree_insert_data(QNode *data_node)
{
    QTree *tree = data_node->tree;

    if(!tree->root)
    {
        int width = 65535;
        int height = 65535;
        int rootx = -width / 2;
        int rooty = -height / 2;
        tree->root = take_qnode_leaf(tree, rootx, rooty, width, height, 0);
    }

    qnode_insert(tree->root, data_node);
}

static QNode *take_qnode_leaf(QTree *tree, int x, int y, int width, int height, size_t depth)
{
    QNode *node = mem_pool_take(tree->node_pool);

    node->type = QNODE_LEAF;
    node->children = NULL;
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->count = 0;
    node->depth = depth;
    node->tree = tree;
    node->parent = NULL;
    node->next = NULL;

    return node;
}
static QNode *take_qnode_data(QTree *tree, int x, int y, void *data)
{
    QNode *node = mem_pool_take(tree->node_pool);

    node->type = QNODE_DATA;
    node->data = data;
    node->x = x;
    node->y = y;
    node->width = 0;
    node->height = 0;
    // not used for data node->area = area;
    node->count = 0;
    //node->depth = depth;
    node->tree = tree;
    node->parent = NULL;
    node->next = NULL;

    return node;
}

static bool qnode_in_region(QNode *node, int x, int y)
{
    int x0 = node->x;
    int x1 = node->x + node->width;
    int y0 = node->y;
    int y1 = node->y + node->height;

    return (x0 <= x && x < x1 && y0 <= y && y < y1);
}

static bool qnode_insert(QNode *parent, QNode *leaf)
{
    assert(parent->type != QNODE_DATA);
    assert(leaf->type == QNODE_DATA);

    size_t max_depth = parent->tree->max_depth;

    if(!qnode_in_region(parent, leaf->x, leaf->y))
            return false;

    // form a really big linked list if we are at maximum depth
    if(parent->depth == max_depth)
    {
        assert(parent->type == QNODE_LEAF);

        leaf->parent = parent;
        leaf->next = parent->children;
        parent->children = leaf;

        return true;
    }

    if(parent->type == QNODE_PARENT)
    {
        QNode *node = parent->children;

        while(node)
        {
            if(qnode_insert(node, leaf))
                return true;

            node = node->next;
        }

        return false;
    }
    else if(parent->count < QNODE_CAPACITY)
    {
        leaf->depth = parent->depth + 1;

        leaf->next = parent->children;
        leaf->parent = parent;
        parent->children = leaf;
        parent->count++;
        return true;
    }
    else
    {
        qnode_subdivide(parent);
        return qnode_insert(parent, leaf);
    }

    return false;
        
}

static void qnode_subdivide(QNode *parent)
{
    QTree *tree = parent->tree;
    size_t depth = parent->depth;

    int x0 = parent->x;
    int y0 = parent->y;
    int width = parent->width / 2;
    int height = parent->width / 2;

    int p0x = x0;
    int p0y = y0;
    int p1x = x0 + width;
    int p1y = y0 + height;

    QNode *nodes[QNODE_CAPACITY];

    nodes[0] = take_qnode_leaf(tree, p0x, p1y, width, height, depth + 1);
    nodes[1] = take_qnode_leaf(tree, p0x, p0y, width, height, depth + 1);
    nodes[2] = take_qnode_leaf(tree, p1x, p1y, width, height, depth + 1);
    nodes[3] = take_qnode_leaf(tree, p1x, p0y, width, height, depth + 1);

    // move all children to subnodes
    QNode *child = parent->children;

    //attach all the leaves to the children
    while(child)
    {
        QNode *next = child->next;
        bool moved = false;
        for(int i = 0; i < QNODE_CAPACITY; ++i)
        {
            if(qnode_in_region(nodes[i], child->x, child->y))
            {
                qnode_detach_data(child);
                qnode_insert(nodes[i], child);
                moved = true;
                break;
            }
        }

        assert(moved);

        child = next;
    }

    parent->type = QNODE_PARENT;
    parent->children = nodes[0];
    nodes[0]->next = nodes[1];
    nodes[1]->next = nodes[2];
    nodes[2]->next = nodes[3];
    nodes[3]->next = NULL;
    nodes[0]->parent = parent;
    nodes[1]->parent = parent;
    nodes[2]->parent = parent;
    nodes[3]->parent = parent;
}

static bool qnode_detach_data(QNode *node)
{
    assert(node->type == QNODE_DATA);
    QNode *parent = node->parent;

    if(!parent){
        return false;
    }

    if(parent->children == node)
    {
        node->parent = NULL;
        parent->children = node->next;
        parent->count--;
        return true;
    }
    else
    {
        QNode *last = parent->children;

        while(last)
        {
            if(last->next == node)
            {
                last->next = node->next;
                parent->count--;
                node->parent = NULL;
                return true;
            }

            last = last->next;
        }
    }

    return false;
}






