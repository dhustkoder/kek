#include "kek.h"

static struct mem_pool pool;
static struct shader *shader = NULL;
#define KEK_VERTEX_BUFFER_CAPACITY (1024*1024)

struct vertex {
    union vec3 position;
    union vec3 normal;
    union vec2 uv;
    union vec4 colormask;
};

static void sprite_fill(union vec2 position, union vec2 size, union vec3 rotation, struct vertex *out);
static void render_default_draw(struct render *render,  struct camera *camera, struct entity *entity, void *ctx);

void render_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct render));
}

struct render *render_create(void)
{
     struct render *render = mem_pool_take(&pool);

     render->draw_callback = NULL;
     render->ctx = NULL;
     render->draw_callback = render_default_draw;

     render->vb = vertex_buffer_create(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,2,4};

    vertex_buffer_set_attribs(render->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(shader == NULL) 
    {
        shader = shader_create();
        shader_load_files(shader, "res/shader/default.vs", "res/shader/default.fs");
    }
    render->shader = shader;

    return render;
}

void render_destroy(struct render *render)
{
    mem_pool_release(&pool, render);
}

void render_set_draw_callback(struct render *render, KEKRenderFn fn, void *ctx)
{
    render->draw_callback = fn;
    render->ctx = ctx;
}

void render_draw(struct render *render, struct camera *camera, struct entity *entity)
{
    if(!render || !render->draw_callback)
        return;

    render->draw_callback(render, camera, entity, render->ctx);
}

void render_default_draw(struct render *render, struct camera *camera, struct entity *entity, void *ctx)
{
    union vec3 size = entity->size;
    struct mat4 mvp;
    struct vertex_buffer *vb = render->vb;
    GLuint shader = render->shader->shader;

    size = vec3_mulf(size, 4.f);
    struct vertex vertices[6]; 
    sprite_fill(entity->position.xy, size.xy, entity->rotation, vertices);


    struct animation_frame *frame = entity_get_animation_frame(entity);

    if(frame)
        texture_bind(frame->texture, 0);

    else if(entity->texture)
        texture_bind(entity->texture, 0);


    vertex_buffer_fill(vb, (uint8_t *)vertices, sizeof(vertices));

    vertex_buffer_bind(vb);
    shader_bind(render->shader);

    {
        gl_uint uid = gl_get_uniform_location(shader, "u_texture");
        gl_uniform1i(uid, 0);
    }

    {
        camera_get_ortho_mvp(camera, &mvp);
        gl_uint uid = gl_get_uniform_location(shader, "u_mvp");
        gl_uniform_matrix4fv(uid, 1, GL_FALSE, (float *)&mvp);
    }
//
    gl_disable(GL_DEPTH_TEST);
    gl_enable(GL_BLEND);
    gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vertex_buffer_draw(vb, 0, vb->size/sizeof(struct vertex));
}

static void sprite_fill(union vec2 position, union vec2 size, union vec3 rotation, struct vertex *out)
{
    union vec2 p0 = position;
    union vec2 p1 = vec2_add(p0, size);
    union vec2 hs = vec2_mulf(size, 0.5f);

	const float x0 = -hs.x;
	const float x1 =  hs.x;
	const float y0 = -hs.y; 
	const float y1 =  hs.y; 
	const float z0 = 0.0f;
	const float z1 = 0.0f; 
    const union vec2 uv0 = {0.0f, 0.0f};
    const union vec2 uv1 = {1.0f, 1.0f};

	const union vec4 positions[] = { 
		{x0, y0, z0, 0}, {x1, y0, z0, 0}, {x1, y1, z0, 0}, 
		{x0, y0, z0, 0}, {x1, y1, z0, 0}, {x0, y1, z1, 0}
	};

	const union vec2 uvs[] = { 
		{uv0.x, uv0.y}, {uv1.x, uv0.y}, {uv1.x, uv1.y}, 
		{uv0.x, uv0.y}, {uv1.x, uv1.y}, {uv0.x, uv1.y}
	};

    struct vertex vertices[6];
    
    struct mat4 rot = mat4_identity();
    rot = mat4_mul(rot, rotatex(rotation.x));
    rot = mat4_mul(rot, rotatey(rotation.y));
    rot = mat4_mul(rot, rotatez(rotation.z));
    
    for(int32_t i = 0; i < 6; ++i)
    {
        union vec4 rp = mat4_mul_vec4(rot, positions[i]);
        union vec3 vert_position = vec3_add(vec2to3(position), rp.xyz);

        vertices[i].position  = vert_position;
        vertices[i].uv        = uvs[i];
        vertices[i].normal    = vec3(0,0,1);
        vertices[i].colormask = vec4(1,1,1,1);
    }

    memcpy(out, vertices, sizeof(vertices));
}

