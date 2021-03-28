#include "kek.h"
#include <stdlib.h>
#include <assert.h>

typedef struct draw_element {
    int camera;
    int render;
    int texture;
    size_t offset;
    size_t count;
} DrawElement;

typedef struct vb_element{
    int64_t order;
    Vec4 colormask;
    Vec3 position;
    Vec3 size;
    Vec3 rotation;
    Vec2 uv0;
    Vec2 uv1;
    int texture;
    int render;
    int camera;
    uint32_t flags;
} VBElement;

#define DRAW_FLAG_NONE         0x00
#define DRAW_FLAG_RENDER_BEGIN 0x01
#define DRAW_FLAG_TEXTURE_SWAP 0x02
#define DRAW_FLAG_CAMERA_SWAP  0x04
#define DRAW_FLAG_RENDER_END   0x08
#define VB_QUEUE_CAPACITY 16384
static VBElement vbqueue[VB_QUEUE_CAPACITY];
static size_t vbqueue_count = 0;

static int sortlist_sort(const void *a, const void *b);
static void fill_sprite(Vec2 position, Vec2 size, Vec3 rotation, Vec2 uv0, Vec2 uv1, Vec4 colormask, Vertex *out);
static void draw_submit_element(VBElement *element);

void init_draw(void)
{
    vbqueue_count = 0;
}

void draw(void)
{
    // sort all the objects by renderer and texture
    VBElement **sortlist = memstack_push(vbqueue_count * sizeof(DrawElement *));

    for(size_t i = 0; i < vbqueue_count; ++i)
        sortlist[i] = &vbqueue[i];

    qsort(sortlist, vbqueue_count, sizeof(VBElement *), sortlist_sort);

    // fill the vbos
    for(size_t i = 0; i < vbqueue_count; ++i)
    {
        VBElement *element = sortlist[i];
        Render *render = get_render(element->render); 
        
        map_vertexbuffer(render->vb);
        clear_vertexbuffer(render->vb);

        Vertex vertices[6];

        fill_sprite(
                element->position.xy, 
                element->size.xy, 
                element->rotation, 
                element->uv0, 
                element->uv1, 
                element->colormask, 
                vertices);

        append_vertexbuffer(
                render->vb,
                (uint8_t *)vertices, 
                sizeof(vertices));

        unmap_vertexbuffer(render->vb);


        int vb           = render->vb;
        int shaderid     = render->shader;
        int textureid    = element->texture;
        Shader *shader   = get_shader(render->shader);
        GLuint program   = shader->shader;
        int camera       = element->camera;
        Mat4 mvp;

        bind_vertexbuffer(vb);
        bind_shader(shaderid);

        bind_texture(textureid, 0);
        gl_uint utexture = gl_get_uniform_location(program, "u_texture");
        gl_uniform1i(utexture, 0);

        get_camera_ortho_mvp(camera, &mvp);
        gl_uint umvp = gl_get_uniform_location(program, "u_mvp");
        gl_uniform_matrix4fv(umvp, 1, GL_FALSE, (float *)&mvp);

        gl_disable(GL_DEPTH_TEST);
        gl_enable(GL_BLEND);
        gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        draw_vertexbuffer(vb, 0, 6);
    }

    vbqueue_count = 0;
}

void draw_submit_tilemap(int render, int tilemapid, int camera)
{
    Tilemap *tilemap = get_tilemap(tilemapid);
    Vec3 campos = get_camera_position(camera);

    float zoom = get_camera_zoom(camera);

    unsigned int window_width = 0;
    unsigned int window_height = 0;

    get_window_size(&window_width, &window_height);

    int x0 = campos.x - window_width * 0.5f;
    int x1 = campos.x + window_width * 0.5f;
    int y0 = campos.y - window_height * 0.5f;
    int y1 = campos.y + window_height * 0.5f;
    x0 /= tilemap->cell_size.x;
    x1 /= tilemap->cell_size.x;
    y0 /= tilemap->cell_size.y;
    y1 /= tilemap->cell_size.y;
    x0 /= zoom;
    x1 /= zoom;
    y0 /= zoom;
    y1 /= zoom;

    x0 = KEK_MAX(x0, 0);
    y0 = KEK_MAX(y0, 0);
    x1 = KEK_MIN(x1, tilemap->num_cells_x - 1);
    y1 = KEK_MIN(y1, tilemap->num_cells_y - 1);

    Vec3 size;
    size.xy = tilemap->cell_size;
    size.z = 0;

    //todo: account for zoom 
    for(int y = y0; y <= y1; ++y)
    {
        for(int x = x0; x <= x1; ++x)
        {
            VBElement element;
            Vec3 position = vec3(x * size.x, y * size.y, 0);

            TilemapSpritesheetClip clip;
            clip = get_tilemap_cell_clip(tilemapid, x, y);

            element.render = render;
            element.uv0 = clip.uv0;
            element.uv1 = clip.uv1;
            element.texture = clip.spritesheet;
            element.colormask = vec4(1,1,1,1);
            element.size = size;
            element.position = position;
            element.rotation = vec3(0,0,0);
            element.camera = camera;

            assert((uint32_t)(element.render) < 0xFFFF);
            assert((uint32_t)(element.texture) < 0xFFFF);

            element.order = 0;
            element.order |= (uint32_t)(element.render << 16);
            element.order |= (uint32_t)(element.texture & 0xFFFF);

            draw_submit_element(&element);
        }
    }
}

void draw_submit_entity(int render, int entityid, int camera)
{
    Entity *entity = get_entity(entityid);
    AnimationFrame *frame = get_entity_animation_frame(entityid);
    VBElement element;

    if(entity->render != RENDER_DEFAULT)
        render = entity->render;

    element.render = render;
    element.uv0 = vec2(0.0f, 0.0f);
    element.uv1 = vec2(1.0f, 1.0f);
    element.colormask = entity->colormask;
    element.texture = entity->texture;
    element.size = entity->scale; 
    element.position = entity->position;
    element.rotation = entity->rotation;
    element.camera = camera;

    assert((uint32_t)(element.render) < 0xFFFF);
    assert((uint32_t)(element.texture) < 0xFFFF);

    element.order = 0;
    element.order |= (int64_t)(entity->layer & 0xFFFF) << 32;
    element.order |= (uint32_t)(element.render & 0xFFFF) << 16;
    element.order |= (uint32_t)(element.texture & 0xFFFF) << 0; 

    if(frame)
    {
        element.uv0 = frame->uv0;
        element.uv1 = frame->uv1;
            
        element.texture = frame->texture;
        element.size.x *= frame->pixel_width;
        element.size.y *= frame->pixel_height;
    }

    if(frame || entity->texture >= 0)
    {
        Vec3 rot = entity->rotation;
        Vec3 trot = entity->texture_rotation;

        element.rotation = add_vec3(rot, trot);
    }

    draw_submit_element(&element);
}

static int sortlist_sort(const void *a, const void *b)
{
    VBElement **aelement = (VBElement **)a;
    VBElement **belement = (VBElement **)b;

    if((*aelement)->order > (*belement)->order)
        return 1;
    if((*aelement)->order < (*belement)->order)
        return -1;
    return 0;
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

static void draw_submit_element(VBElement *element)
{
    assert(vbqueue_count < VB_QUEUE_CAPACITY);

    vbqueue[vbqueue_count] = *element;

    ++vbqueue_count;
}

