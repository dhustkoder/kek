#include "kek.h"

typedef struct draw {
    GLuint texture;
    size_t offset;
    size_t count;
} Draw;

static void fill_sprite(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out);
static void fill_sprite_box(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out);
static void fill_circle(Vec2 position, float radius, Vec4 colormask, CircleVertex *out);

void draw_render_entities(Render *render, int camera, int *entities, size_t count, void *ctx)
{
    size_t drawcount = 0;
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    Draw *draw_start = memstack_push(sizeof(Draw));
    Draw *draw_top = draw_start;
    draw_top->texture = 0;
    draw_top->offset = 0;
    draw_top->count = 0;
    ++drawcount;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        int entityid = entities[i];

        AnimationFrame *frame = get_entity_animation_frame(entityid);
        Entity *entity = get_entity(entityid);
        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {1.0f, 1.0f};
        Vec4 colormask = entity->colormask;
        int texture = entity->texture;

        if(frame)
        {
            uv0 = frame->uv0;
            uv1 = frame->uv1;
            
            texture = frame->texture;
        }

        if(frame || entity->texture >= 0)
        {
            Vertex vertices[6];
            Vec3 size = mul_vec3_f(entity->size, 4.f);

            Vec3 rot = add_vec3(entity->rotation, entity->texture_rotation);
            fill_sprite(entity->position.xy, size.xy, rot, uv0, uv1, colormask, vertices);
            append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));

            if(texture != draw_top->texture)
            {
                size_t offset = draw_top->offset + draw_top->count;

                draw_top = memstack_push(sizeof(Draw));
                draw_top->texture = texture;
                draw_top->offset = (int)offset;
                draw_top->count = 6;
                drawcount++;
            }
            else
            {
                draw_top->count += 6;
            }
        }
    }
    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);
    for(int i = 0; i < drawcount; ++i)
    {
        Draw *draw = &draw_start[i];
        Mat4 mvp;

        bind_texture(draw->texture, 0);
        gl_uint utexture = gl_get_uniform_location(program, "u_texture");
        gl_uniform1i(utexture, 0);

        get_camera_ortho_mvp(camera, &mvp);
        gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
        gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
        gl_disable(GL_DEPTH_TEST);
        gl_enable(GL_BLEND);
        gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        if(draw->count)
            draw_vertexbuffer(vb, draw->offset, draw->count);
    }

    memstack_pop(draw_start);
}

void draw_render_entity_boxes(Render *render, int camera, int *entities, size_t count, void *ctx)
{
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        int entityid = entities[i];
        Entity *entity = get_entity(entityid);

        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {1.0f, 1.0f};
        Vec4 colormask = vec4(1,1,1,0.5);//entity->colormask;
        int texture = entity->texture;

        Vertex vertices[12];
        Vec3 size = entity->size;

        fill_sprite_box(entity->position.xy, size.xy, entity->rotation, uv0, uv1, colormask, vertices);
        append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));

        vertexcount += 12;
    }

    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);

    Mat4 mvp;

   get_camera_ortho_mvp(camera, &mvp);
   gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
   gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
   gl_disable(GL_DEPTH_TEST);
   gl_enable(GL_BLEND);
   gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
   if(vertexcount > 0)
       draw_vertexbuffer_lines(vb, 0, vertexcount);

}

void draw_render_collision_circles(Render *render, int camera, int *entities, size_t count, void *ctx)
{
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        int entityid = entities[i];
        Entity *entity = get_entity(entityid);

        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {1.0f, 1.0f};
        Vec4 colormask = vec4(1,1,1,0.5);//entity->colormask;
        int texture = entity->texture;

        CircleVertex vertices[6];

        if(entity->collider.type == COLLIDER_CIRCLE)
        {
            float radius = entity->collider.radius;

            fill_circle(entity->position.xy, radius, colormask, vertices);
            append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));

            vertexcount += 6;
        }
    }

    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);

    Mat4 mvp;

   get_camera_ortho_mvp(camera, &mvp);
   gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
   gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
   gl_disable(GL_DEPTH_TEST);
   gl_enable(GL_BLEND);
   gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
   if(vertexcount > 0)
       draw_vertexbuffer(vb, 0, vertexcount);

}

void draw_render_collision_boxes(Render *render, int camera, int *entities, size_t count, void *ctx)
{
    size_t vertexcount = 0;
    int vb = render->vb;
    Shader *shader = get_shader(render->shader);
    GLuint program = shader->shader;

    map_vertexbuffer(vb);
    clear_vertexbuffer(vb);

    for(size_t i = 0; i < count; ++i)
    {
        int entityid = entities[i];
        Entity *entity = get_entity(entityid);

        Vec2 uv0 = {0.0f, 0.0f};
        Vec2 uv1 = {1.0f, 1.0f};
        Vec4 colormask = vec4(1,1,1,0.5);//entity->colormask;
        int texture = entity->texture;


        if(entity->collider.type == COLLIDER_RECT)
        {
            Vertex vertices[6];

            Vec3 size = vec2to3(entity->collider.rect);

            fill_sprite(entity->position.xy, size.xy, entity->rotation, uv0, uv1, colormask, vertices);
            append_vertexbuffer(vb, (uint8_t *)vertices, sizeof(vertices));

            vertexcount += 6;
        }
    }

    unmap_vertexbuffer(vb);

    bind_vertexbuffer(vb);
    bind_shader(render->shader);

    Mat4 mvp;

   get_camera_ortho_mvp(camera, &mvp);
   gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
   gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);
    
   gl_disable(GL_DEPTH_TEST);
   gl_enable(GL_BLEND);
   gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
   if(vertexcount > 0)
       draw_vertexbuffer(vb, 0, vertexcount);

}


static void fill_sprite(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out)
{
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

static void fill_sprite_box(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out)
{
    Vec2 hs = mul_vec2_f(size, 0.5f);

	const float x0 = -hs.x;
	const float x1 =  hs.x;
	const float y0 = -hs.y; 
	const float y1 =  hs.y; 
	const float z0 = 0.0f;
	const float z1 = 0.0f; 

	const Vec4 positions[] = { 
		{x0, y0, z0, 0},  {x1, y0, z0, 0}, 
		{x1, y0, z0, 0},  {x1, y1, z0, 0}, 
		{x1, y1, z0, 0},  {x0, y1, z0, 0}, 
		{x0, y1, z0, 0},  {x0, y0, z0, 0}, 
		{x0, y0, z0, 0},  {x1, y1, z0, 0}, 
		{x1, y0, z0, 0},  {x0, y1, z0, 0}, 
	};

    Vertex vertices[12];
    
    Mat4 rot = identity_mat4();
    rot = mul_mat4(rot, rotatex(rotation.x));
    rot = mul_mat4(rot, rotatey(rotation.y));
    rot = mul_mat4(rot, rotatez(rotation.z));
    
    for(int32_t i = 0; i < 12; ++i)
    {
        Vec4 rp = mul_mat4_vec4(rot, positions[i]);
        Vec3 vert_position = add_vec3(vec2to3(position), rp.xyz);

        vertices[i].position  = vert_position;
        vertices[i].uv        = vec2(0,0);
        vertices[i].normal    = vec3(0,0,1);
        vertices[i].colormask = colormask;
    }

    memcpy(out, vertices, sizeof(vertices));
}

static void fill_circle(Vec2 position, float radius, Vec4 colormask, CircleVertex *out)
{
	const float x0 = -radius;
	const float x1 =  radius;
	const float y0 = -radius; 
	const float y1 =  radius; 
	const float z0 = 0.0f;
	const float z1 = 0.0f; 

	const Vec3 positions[] = { 
		{x0, y0, z0}, {x1, y0, z0}, {x1, y1, z0}, 
		{x0, y0, z0}, {x1, y1, z0}, {x0, y1, z1}
	};

    CircleVertex vertices[6];
    
    for(int32_t i = 0; i < 6; ++i)
    {
        Vec3 pv = vec2to3(position);
        pv = add_vec3(pv, positions[i]);

        vertices[i].position  = pv;
        vertices[i].center = vec2to3(position);
        vertices[i].radius = radius;
        vertices[i].colormask = colormask;
    }

    memcpy(out, vertices, sizeof(vertices));
}
