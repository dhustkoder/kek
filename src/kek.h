#pragma once
#include "kek_type.h"
#include "glad/gl.h"
#include <stdarg.h>
#include <string.h>

//**********************************************************
// KEK GENERAL 
//**********************************************************
void  quit_kek(void);
void  framerate(int fps);
float get_frame_time(void);
float get_target_frame_interval(void);

//**********************************************************
// KEK CONFIG
//**********************************************************
void init_config(void);
void configi(ConfigType type, int value);
int  get_configi(ConfigType type);

//**********************************************************
// KEK WINDOW 
//**********************************************************
int        init_window(void);
int        window_title(const char *title);
int        window_size(unsigned int width, unsigned int height);
int        get_window_size(unsigned int *width, unsigned int *height);
int        open_window(const char *title, unsigned int width, unsigned int height);
int        poll_window(void);
int        swap_window_buffers(void);
PALWindow *get_pal_window(void);
void       clearscreen(void);
void       clearscreen_color(Vec4 color);

//**********************************************************
// KEK LOG 
//**********************************************************
#define LOG_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
int     log_line(const char *file, int line, const char *function, const char *zone, const char *format, ...);
#define log(...) log_line(LOG_FILENAME,  __LINE__, __FUNCTION__, "INFO", __VA_ARGS__)
#define logd(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "DEBUG", __VA_ARGS__)
#define logw(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "WARN", __VA_ARGS__)
#define loge(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "ERROR", __VA_ARGS__)

//**********************************************************
// KEK MEMORY 
//**********************************************************
void  mem_pool_alloc(MemPool *pool, size_t capacity, size_t stride);
void *mem_pool_take(MemPool *pool);
void *mem_pool_release(MemPool *pool, void *addr);
void  mem_pool_free(MemPool *pool);

void  mem_stack_init(size_t capacity);
void *mem_stack_push(size_t size);
void  mem_stack_pop(void *addr);
void  mem_stack_free(void);

//**********************************************************
// KEK SPATIAL MAP 
//**********************************************************
void spatialmap_init(size_t capacity);
SpatialMap *spatialmap_create(SpatialNode **node_list, MemPool *node_pool, size_t xbits, size_t ybits);
SpatialNode *spatialmap_create_node(SpatialMap *map, void *data);
void spatialmap_destroy_node(SpatialNode *node);
SpatialNode *spatialmap_move_node(SpatialNode *node, int x, int y);
void spatialmap_query(SpatialMap *spatial, int x0, int y0, int x1, int y1, SpatialMapQueryFn fn, void *ctx);


//**********************************************************
// KEK VERTEX BUFFER
//**********************************************************
void          init_vertex_buffer(size_t capacity);
VertexBuffer *create_vertex_buffer(size_t capacity);
void          destroy_vertex_buffer(VertexBuffer *vb);
void          vertex_buffer_attribs(VertexBuffer *vb, size_t *attribs, size_t count);
size_t        get_vertex_buffer_capacity(VertexBuffer *vb);
size_t        get_vertex_buffer_size(VertexBuffer *vb);
void          clear_vertex_buffer(VertexBuffer *vb);
int           fill_vertex_buffer(VertexBuffer *vb, uint8_t *data, size_t size);
void          bind_vertex_buffer(VertexBuffer *vb);
void          draw_vertex_buffer(VertexBuffer *vb, size_t start, size_t count);
int           map_vertex_buffer(VertexBuffer *vb);
int           unmap_vertex_buffer(VertexBuffer *vb);
void          append_vertex_buffer_data(VertexBuffer *vb, uint8_t *data, size_t vertices);

//**********************************************************
// KEK SHADER
//**********************************************************
void    init_shader(size_t capacity);
Shader *create_shader(void);
void    destroy_shder(Shader *shader);
int     load_shader_files(Shader *shader, const char *vert_file, const char *frag_file);
int     load_shader_buffer(Shader *shader, const char *vert_buffer, const char *frag_buffer);
int     bind_shader(Shader *shader);

#if 0
//**********************************************************
// KEK MATERIAL
//**********************************************************
void material_init(size_t capacity);
Material *material_create(void);
void material_add_property(Material *material, const MaterialProperty *property);
void material_destroy(void);
#endif


//**********************************************************
// KEK TEXTURE 
//**********************************************************
void     init_texture(size_t capacity);
Texture *create_texture(void);
void     destroy_texture(Texture *texture);
int      load_texture_file(Texture *texture, const char *file);
void     bind_texture(Texture *textures, int slot);

//**********************************************************
// KEK ANIMATION
//**********************************************************
void       init_animation(size_t capacity);
Animation *create_animation(void);
void       destroy_animation(Animation *animation);
void       add_animation_frame(Animation *animation, AnimationFrame frame);
void       add_animation_frame_clip(Animation *animation, Texture *texture, int x, int y, int clip_width, int clip_height, float duration);
void       animation_loop(Animation *animation, bool loop);

//**********************************************************
// KEK EVENT 
//**********************************************************
void init_event(size_t capacity);
void register_event(int id, EventCallback cb, void *ctx); 
void submit_event(int id, void *data);

//**********************************************************
// KEK RENDER
//**********************************************************
void    init_render(size_t capacity);
Render *create_render(void);
void    destroy_render(Render *render);
void    render_draw_callback(Render *render, RenderFn fn, void *ctx);
void    draw_render(Render *render, Camera *camera, Entity *entity);
//
//**********************************************************
// KEK CAMERA 
//**********************************************************
void    init_camera(size_t capacity);
Camera *create_camera(void);
void    destroy_camera(Camera *camera);
void    get_camera_ortho_mvp(Camera *camera, Mat4 *mvp);
Vec2    get_camera_mouse_position(Camera *camera);
void    camera_ortho_zoom(Camera *camera, float zoom);
void    camera_position(Camera *camera, Vec3 position);

//**********************************************************
// KEK SCENE 
//**********************************************************
void    init_scene(size_t capacity);
Scene  *create_scene(void);
void    destroy_scene(Scene *scene);
void    scene_camera(Scene *scene, Camera *camera);
Camera *get_scene_camera(Scene *scene);
void    scene_active(Scene *scene);
Scene  *get_active_scene(void);
void    update_scene(Scene *scene);
void    draw_scene(Scene *scene);
void    add_scene_entity(Scene *scene, Entity *entity);
void    garbage_collect_scene(Scene *scene);
void    query_scene_entities(Scene *scene, Vec2 pos, Vec2 size, SceneQueryEntityFn fn, void *ctx);

//**********************************************************
// KEK ENTITY
//**********************************************************
void            init_entity(size_t capacity, size_t type_capacity, size_t user_data_stride);
Entity         *create_entity(uint32_t type);
void            destroy_entity(Entity *entity);
void            release_entity(Entity *entity);
void            entity_update_callback(uint32_t type, EntityUpdateFn callback);
void            entity_terminate_callback(uint32_t type, EntityTerminateFn callback);
void            entity_callback_context(uint32_t type, void *ctx);
void           *get_entity_user_data(Entity *entity);
void            update_entity(Entity *e);
Vec3            get_entity_size(Entity *e);
void            entity_size(Entity *e, Vec3 size);
Vec3            get_entity_position(Entity *e);
void            entity_position(Entity *e, Vec3 position);
Vec3            get_entity_velocity(Entity *e);
void            entity_velocity(Entity *e, Vec3 velocity);
void            entity_texture(Entity *e, Texture *texture);
void            entity_animation(Entity *e, Animation *animation);
void            entity_rotation(Entity *e, Vec3 rotation);
void            entity_rotation_z(Entity *e, float rotation);
void            reset_entity_animation(Entity *e);
void            set_entity_animation_speed(Entity *e, float speed);
AnimationFrame *get_entity_animation_frame(Entity *e);
void            query_entity(Vec2 p0, Vec2 p1, EntityQueryFn fn, void *ctx);

void               init_physics(void);
void               simulate_physics(void);              
PhysicsBody       *create_physics_circle_body(Vec3 position, float radius, uint32_t type);
void               destroy_physics_body(PhysicsBody *body);
void               physics_body_position(PhysicsBody *e, Vec2 pos);
Vec2               get_physics_body(PhysicsBody *e);

//**********************************************************
// KEK KEY 
//**********************************************************
void     init_tag(size_t capacity);
uint32_t set_tag(uint32_t tag, void *value);
void    *get_tag(uint32_t tag);
void     remove_tag(uint32_t tag);

//**********************************************************
// KEK HID 
//**********************************************************
void init_hid(size_t alias_capacity);
void bind_hid_alias_to_key(int alias, KeyboardKey key);
void unbind_hid_key(KeyboardKey key);
void unbind_hid_alias(int alias);
bool is_hid_alias_pressed(int alias);
bool is_hid_key_pressed(KeyboardKey key);
bool is_mouse_button_pressed(MouseButton button);
Vec2 get_mouse_position(void);

//**********************************************************
// KEK RNG 
//**********************************************************
void         init_random(void);
int          get_random_rangei(int min, int max);
unsigned int get_randomui(void);
float        get_random_rangef(float min, float max);
Vec2         get_random_range_v2(Vec2 min, Vec2 max);
Vec3         get_random_range_v3(Vec3 min, Vec3 max);
Vec4         get_random_range_v4(Vec4 min, Vec4 max);

//**********************************************************
// KEK MATH 
//**********************************************************

//**********************************************************
// KEK MATH 
//**********************************************************
unsigned int aabb(Vec2 p0, Vec2 s0, Vec2 p1, Vec2 s1);
unsigned int aabb2(Rect2 a, Rect2 b);
unsigned int aabb3(Cube a, Cube b);
Vec2 add_vec2(Vec2 a, Vec2 b);
Vec2 sub_vec2(Vec2 a, Vec2 b);
Vec2 mul_vec2(Vec2 a, Vec2 b);
Vec2 div_vec2(Vec2 a, Vec2 b);
Vec2 add_vec2_f(Vec2 a, float b);
Vec2 sub_vec2_f(Vec2 a, float b);
Vec2 mul_vec2_f(Vec2 a, float b);
Vec2 div_vec2_f(Vec2 a, float b);
Vec3 add_vec3(Vec3 a, Vec3 b);
Vec3 sub_vec3(Vec3 a, Vec3 b);
Vec3 mul_vec3(Vec3 a, Vec3 b);
Vec3 div_vec3(Vec3 a, Vec3 b);
Vec3 add_vec3_f(Vec3 a, float b);
Vec3 sub_vec3_f(Vec3 a, float b);
Vec3 mul_vec3_f(Vec3 a, float b);
Vec3 div_vec3_f(Vec3 a, float b);
Vec4 add_vec4(Vec4 a, Vec4 b);
Vec4 sub_vec4(Vec4 a, Vec4 b);
Vec4 mul_vec4(Vec4 a, Vec4 b);
Vec4 div_vec4(Vec4 a, Vec4 b);
Vec4 add_vec4_f(Vec4 a, float b);
Vec4 sub_vec4_f(Vec4 a, float b);
Vec4 mul_vec4_f(Vec4 a, float b);
Vec4 div_vec4_f(Vec4 a, float b);
Mat4 mul_mat4(Mat4 a, Mat4 b);
Vec4 mul_mat4_vec4(Mat4 m, Vec4 v);
Mat4 mul_mat4_f(Mat4 m, float v);
Mat4 inverse_mat4(const Mat4 mat);
Mat4 transpose_mat4(Mat4 *m);
Mat4 translate_mat4(Vec3 pos);
Mat4 rotatex(float angle);
Mat4 rotatey(float angle);
Mat4 rotatez(float angle);
Mat4 scale_vec3_to_mat4(Vec3 scale);
Mat4 look_at(Vec3 eye, Vec3 target, Vec3 up);
Mat4 ortho_projection(float left, float right, float bottom, float top, float near, float far);
Mat4 perspective_projection(float fovy, float aspect, float near, float far);
Vec3 unproject(Vec3 pos, Mat4 viewproj, Vec4 viewport);
Vec3 screen_to_world_coord(Mat4 projection, Mat4 view, Vec4 viewport, Vec2 coord);
float clampf(float value, float min, float max);
float deg_to_rad(float deg);
float rad_to_deg(float rad);
int32_t line_intersect_check(Vec3 p0, Vec3 v0, Vec3 p1, Vec3 v1, Vec3 *c);
float dot_product_vec2(Vec2 a, Vec2 b);
float dot_product_vec3(Vec3 a, Vec3 b);
float dot_product_vec4(Vec4 a, Vec4 b);
Vec3 cross_product(Vec3 a, Vec3 b);
Vec2 project_vec2(Vec2 u, Vec2 v);
Vec3 project_vec3(Vec3 u, Vec3 v);
Vec4 project_vec4(Vec4 u, Vec4 v);
float length_vec2(Vec2 v);
float length_vec3(Vec3 v);
float length_vec4(Vec4 v);
float length2_vec2(Vec2 v);
float length2_vec3(Vec3 v);
float length2_vec4(Vec4 v);
Vec2 normal_vec2(Vec2 v);
Vec3 normal_vec3(Vec3 v);
Vec4 normal_vec4(Vec4 v);
Vec2 negative_vec2(Vec2 v);
Vec3 negative_vec3(Vec3 v);
Vec4 negative_vec4(Vec4 v);
int signi(int sign);
Vec2 sign_vec2(Vec2 v);
Vec3 sign_vec3(Vec3 v);
Vec4 sign_vec4(Vec4 v);
Vec2 abs_vec2(Vec2 v);
Vec3 abs_vec3(Vec3 v);
Vec4 abs_vec4(Vec4 v);
Vec2 clamp_vec2(Vec2 v, Vec2 min, Vec2 max);
Vec3 clamp_vec3(Vec3 v, Vec3 min, Vec3 max);
Vec4 clamp_vec4(Vec4 v, Vec4 min, Vec4 max);
Vec2 zero_vec2();
Vec3 zero_vec3();
Vec4 zero_vec4();
Mat4 zero_mat4();
Vec2 identity_vec2();
Vec3 identity_vec3();
Vec4 identity_vec4();
Mat4 identity_mat4();
Vec2 vec2(float x, float y);
Vec3 vec3(float x, float y, float z);
Vec4 vec4(float x, float y, float z, float w);
Vec2 vec3to2(Vec3 v);
Vec2 vec4to2(Vec4 v);
Vec3 vec2to3(Vec2 v);
Vec3 vec4to3(Vec4 v);
Vec4 vec2to4(Vec2 v);
Vec4 vec3to4(Vec3 v);
void order_vec3(Vec3 *p0, Vec3 *p1);
Vec2 line_line_intersect_point(Vec2 p0, Vec2 p1, Vec2 q0, Vec2 q1);
bool circle_circle_intersect(Vec2 p0, float r0, Vec2 p1, float r1);
float line_slope(Vec2 p0, Vec2 p1);
float line_normal_slope(Vec2 p0, Vec2 p1);
bool line_circle_collision(Vec2 p0, Vec2 p1, Vec2 c, float r, Vec2 *intersect);
bool point_line_bounce(Vec2 c0, Vec2 vel, Vec2 p0, Vec2 p1, Vec2 *p_intersect, Vec2 *c_out, Vec2 *dir);

