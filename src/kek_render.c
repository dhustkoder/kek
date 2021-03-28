#include "kek.h"

static MemPool pool;
static int shader = -1;
static int box_shader = -1;
static int circle_shader = -1;
#define KEK_VERTEX_BUFFER_CAPACITY (1024*1024)


void init_render(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Render));
}

int create_render(int shader, const size_t *attrib_sizes, int attrib_count)
{
    Render *inst = mempool_take(&pool);

    inst->id = mempool_get_slot(&pool, inst);
    inst->ctx = NULL;
    inst->vb = create_vertexbuffer(KEK_VERTEX_BUFFER_CAPACITY);
    inst->shader = shader;

    vertexbuffer_attribs(inst->vb, attrib_sizes, attrib_count); 

    return inst->id;
}

int create_entity_render(void)
{
    size_t attribs[] = {3,3,2,4};

    // use a default shader
    // todo: clean this up and create afunction
    if(shader == -1) 
    {
        shader = create_shader();
        load_shader_files(shader, "res/shader/default.vs", "res/shader/default.fs");
    }

    return create_render(shader, attribs, 4);
}

int create_entity_box_render(void)
{
    size_t attribs[] = {3,3,2,4};
    // use a default shader
    // todo: clean this up and create afunction
    if(box_shader == -1) 
    {
        box_shader = create_shader();
        load_shader_files(box_shader, "res/shader/entity_box.vs", "res/shader/entity_box.fs");
    }

    return create_render(box_shader, attribs, 4); 
}

int create_rect_render(void)
{
    size_t attribs[] = {3,3,2,4};


    // use a default shader
    // todo: clean this up and create afunction
    if(box_shader == -1) 
    {
        box_shader = create_shader();
        load_shader_files(box_shader, "res/shader/rect.vs", "res/shader/rect.fs");
    }

    return create_render(box_shader, attribs, 4); 
}


int create_circle_render(void)
{
    size_t attribs[] = {3,3,1,4};

    if(circle_shader == -1) 
    {
        circle_shader = create_shader();
        load_shader_files(circle_shader, "res/shader/circle.vs", "res/shader/circle.fs");
    }

    return create_render(circle_shader, attribs, 4);
}

Render *get_render(int id)
{
    return mempool_get_addr(&pool, id);
}

int get_render_shader(int id)
{
    Render *render = get_render(id);

    return render->shader;
}

void render_shader(int id, int shader)
{
    Render *render = get_render(id);

    render->shader = shader;
}

void destroy_render(int id)
{
    Render *render = mempool_get_addr(&pool, id);
    mempool_release(&pool, render);
}

