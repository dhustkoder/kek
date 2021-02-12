#include "kek.h"

static MemPool pool;
static Shader *shader = NULL;
#define KEK_VERTEX_BUFFER_CAPACITY (1024*1024)

typedef struct vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec4 colormask;
} Vertex;

static void fill_sprite(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out);
static void draw_render_default(Render *render,  Camera *camera, Entity *entity, void *ctx);

void init_render(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Render));
}

Render *create_render(void)
{
     Render *render = mem_pool_take(&pool);

     render->draw_callback = NULL;
     render->ctx = NULL;
     render->draw_callback = draw_render_default;

     render->vb = create_vertex_buffer(KEK_VERTEX_BUFFER_CAPACITY);
    
    size_t attribs[] = {3,3,2,4};

    vertex_buffer_attribs(render->vb, attribs, 4); 

    // use a default shader
    // todo: clean this up and create afunction
    if(shader == NULL) 
    {
        shader = create_shader();
        load_shader_files(shader, "res/shader/default.vs", "res/shader/default.fs");
    }
    render->shader = shader;

    return render;
}

void destroy_render(Render *render)
{
    mem_pool_release(&pool, render);
}

void render_draw_callback(Render *render, RenderFn fn, void *ctx)
{
    render->draw_callback = fn;
    render->ctx = ctx;
}

void draw_render(Render *render, Camera *camera, Entity *entity)
{
    if(!render || !render->draw_callback)
        return;

    render->draw_callback(render, camera, entity, render->ctx);
}

void draw_render_default(Render *render, Camera *camera, Entity *entity, void *ctx)
{
    Vec3 size = entity->size;
    Mat4 mvp;
    VertexBuffer *vb = render->vb;
    GLuint shader = render->shader->shader;

    size = mul_vec3_f(size, 4.f);
    Vertex vertices[6]; 

    AnimationFrame *frame = get_entity_animation_frame(entity);

    if(frame)
    {
        Vec2 uv0 = frame->uv0;
        Vec2 uv1 = frame->uv1;
        Vec4 colormask = entity->colormask;
        
        fill_sprite(entity->position.xy, size.xy, entity->rotation, uv0, uv1, colormask, vertices);
        bind_texture(frame->texture, 0);
    }

    else if(entity->texture)
    {
        const Vec2 uv0 = {0.0f, 0.0f};
        const Vec2 uv1 = {1.0f, 1.0f};
        const Vec4 colormask = entity->colormask;
        fill_sprite(entity->position.xy, size.xy, entity->rotation, uv0, uv1, colormask, vertices);
        bind_texture(entity->texture, 0);
    }


    fill_vertex_buffer(vb, (uint8_t *)vertices, sizeof(vertices));

    bind_vertex_buffer(vb);
    bind_shader(render->shader);

    {
        gl_uint uid = gl_get_uniform_location(shader, "u_texture");
        gl_uniform1i(uid, 0);
    }
    {
        get_camera_ortho_mvp(camera, &mvp);
        gl_uint uid = gl_get_uniform_location(shader, "u_mvp");
        gl_uniform_matrix4fv(uid, 1, GL_FALSE, (float *)&mvp);
    }
//
    gl_disable(GL_DEPTH_TEST);
    gl_enable(GL_BLEND);
    gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    draw_vertex_buffer(vb, 0, vb->size/sizeof(Vertex));
}

static void fill_sprite(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out)
{
    Vec2 p0 = position;
    Vec2 p1 = add_vec2(p0, size);
    Vec2 hs = mul_vec2_f(size, 0.5f);

	const float x0 = -hs.x;
	const float x1 =  hs.x;
	const float y0 = -hs.y; 
	const float y1 =  hs.y; 
	const float z0 = 0.0f;
	const float z1 = 0.0f; 

	const Vec4 positions[] = { 
		{x0, y0, z0, 0}, {x1, y0, z0, 0}, {x1, y1, z0, 0}, 
		{x0, y0, z0, 0}, {x1, y1, z0, 0}, {x0, y1, z1, 0}
	};

	const Vec2 uvs[] = { 
		{uv0.x, uv0.y}, {uv1.x, uv0.y}, {uv1.x, uv1.y}, 
		{uv0.x, uv0.y}, {uv1.x, uv1.y}, {uv0.x, uv1.y}
	};

    Vertex vertices[6];
    
    Mat4 rot = identity_mat4();
    rot = mul_mat4(rot, rotatex(rotation.x));
    rot = mul_mat4(rot, rotatey(rotation.y));
    rot = mul_mat4(rot, rotatez(rotation.z));
    
    for(int32_t i = 0; i < 6; ++i)
    {
        Vec4 rp = mul_mat4_vec4(rot, positions[i]);
        Vec3 vert_position = add_vec3(vec2to3(position), rp.xyz);

        vertices[i].position  = vert_position;
        vertices[i].uv        = uvs[i];
        vertices[i].normal    = vec3(0,0,1);
        vertices[i].colormask = colormask;
    }

    memcpy(out, vertices, sizeof(vertices));
}

