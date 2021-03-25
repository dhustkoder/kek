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

int create_entity_render(void)
{
     Render *inst = mempool_take(&pool);

     inst->id = mempool_get_slot(&pool, inst);

     inst->ctx = NULL;

     inst->vb = create_vertexbuffer(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,2,4};

    vertexbuffer_attribs(inst->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(shader == -1) 
    {
        shader = create_shader();
        load_shader_files(shader, "res/shader/default.vs", "res/shader/default.fs");
    }
    inst->shader = shader;

    return inst->id;
}

Render *create_entity_box_render(void)
{
     Render *render = mempool_take(&pool);

     render->ctx = NULL;

     render->vb = create_vertexbuffer(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,2,4};

    vertexbuffer_attribs(render->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(box_shader == -1) 
    {
        box_shader = create_shader();
        load_shader_files(box_shader, "res/shader/entity_box.vs", "res/shader/entity_box.fs");
    }
    render->shader = box_shader;

    return render;
}

Render *create_rect_render(void)
{
     Render *render = mempool_take(&pool);

     render->ctx = NULL;

     render->vb = create_vertexbuffer(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,2,4};

    vertexbuffer_attribs(render->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(box_shader == -1) 
    {
        box_shader = create_shader();
        load_shader_files(box_shader, "res/shader/rect.vs", "res/shader/rect.fs");
    }
    render->shader = box_shader;

    return render;
}


Render *create_circle_render(void)
{
     Render *render = mempool_take(&pool);

     render->ctx = NULL;

     render->vb = create_vertexbuffer(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,1,4};

    vertexbuffer_attribs(render->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(circle_shader == -1) 
    {
        circle_shader = create_shader();
        load_shader_files(circle_shader, "res/shader/circle.vs", "res/shader/circle.fs");
    }
    render->shader = circle_shader;

    return render;
}

Render *get_render(int id)
{
    return mempool_get_addr(&pool, id);
}

void destroy_render(int id)
{
    Render *render = mempool_get_addr(&pool, id);
    mempool_release(&pool, render);
}

