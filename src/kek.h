#pragma once
#include "kek_type.h"
#include "glad/gl.h"
#include <stdarg.h>
#include <string.h>

//**********************************************************
// KEK GENERAL 
//**********************************************************
void  kek_quit(void);
void  kek_set_framerate(int fps);
float kek_get_frame_time(void);
float kek_get_target_frame_interval(void);

//**********************************************************
// KEK CONFIG
//**********************************************************
void config_init(void);
void config_seti(ConfigType type, int value);
int  config_geti(ConfigType type);

//**********************************************************
// KEK WINDOW 
//**********************************************************
int        window_init(void);
int        window_set_title(const char *title);
int        window_set_size(unsigned int width, unsigned int height);
int        window_get_size(unsigned int *width, unsigned int *height);
int        window_open(const char *title, unsigned int width, unsigned int height);
PALWindow *window_get_pal(void);
void       window_clearscreen(void);
void       window_set_clearscreen_color(Vec4 color);

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
// KEK VERTEX BUFFER
//**********************************************************
void          vertex_buffer_init(size_t capacity);
VertexBuffer *vertex_buffer_create(size_t capacity);
void          vertex_buffer_destroy(VertexBuffer *vb);
void          vertex_buffer_set_attribs(VertexBuffer *vb, size_t *attribs, size_t count);
size_t        vertex_buffer_get_capacity(VertexBuffer *vb);
size_t        vertex_buffer_get_size(VertexBuffer *vb);
void          vertex_buffer_clear(VertexBuffer *vb);
int           vertex_buffer_fill(VertexBuffer *vb, uint8_t *data, size_t size);
void          vertex_buffer_bind(VertexBuffer *vb);
void          vertex_buffer_draw(VertexBuffer *vb, size_t start, size_t count);
int           vertex_buffer_map(VertexBuffer *vb);
int           vertex_buffer_unmap(VertexBuffer *vb);
int           vertex_buffer_append(VertexBuffer *vb, uint8_t *data, size_t vertices);

//**********************************************************
// KEK SHADER
//**********************************************************
void    shader_init(size_t capacity);
Shader *shader_create(void);
void    shader_destroy(Shader *shader);
int     shader_load_files(Shader *shader, const char *vert_file, const char *frag_file);
int     shader_load_buffer(Shader *shader, const char *vert_buffer, const char *frag_buffer);
int     shader_free(Shader *shader);
int     shader_bind(Shader *shader);

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
void     texture_init(size_t capacity);
Texture *texture_create(void);
void     texture_destroy(Texture *texture);
int      texture_load_file(Texture *texture, const char *file);
void     texture_bind(Texture *textures, int slot);

//**********************************************************
// KEK ANIMATION
//**********************************************************
void       animation_init(size_t capacity);
Animation *animation_create(void);
void       animation_destroy(Animation *animation);
void       animation_add_frame(Animation *animation, AnimationFrame frame);
void       animation_add_frame_clip(Animation *animation, Texture *texture, int x, int y, int clip_width, int clip_height, float duration);
void       animation_set_loop(Animation *animation, bool loop);

//**********************************************************
// KEK RENDER
//**********************************************************
void    render_init(size_t capacity);
Render *render_create(void);
void    render_destroy(Render *render);
void    render_set_draw_callback(Render *render, RenderFn fn, void *ctx);
void    render_draw(Render *render, Camera *camera, Entity *entity);
//
//**********************************************************
// KEK CAMERA 
//**********************************************************
void    camera_init(size_t capacity);
Camera *camera_create(void);
void    camera_destroy(Camera *camera);
void    camera_set_ortho_zoom(Camera *camera, float zoom);
void    camera_get_ortho_mvp(Camera *camera, Mat4 *mvp);
void    camera_set_position(Camera *camera, Vec3 position);
Vec2    camera_get_mouse_position(Camera *camera);

//**********************************************************
// KEK SCENE 
//**********************************************************
void    scene_init(size_t capacity);
Scene  *scene_create(void);
void    scene_destroy(Scene *scene);
void    scene_set_camera(Scene *scene, Camera *camera);
Camera *scene_get_camera(Scene *scene);
void    scene_set_active(Scene *scene);
Scene  *scene_get_active(void);
void    scene_update(Scene *scene);
void    scene_draw(Scene *scene);
void    scene_add_entity(Scene *scene, Entity *entity);
void    scene_garbage_collect(Scene *scene);
void    scene_query_entities_aabb(Scene *scene, Vec2 pos, Vec2 size, SceneQueryEntityFn fn, void *ctx);

//**********************************************************
// KEK ENTITY
//**********************************************************
void            entity_init(size_t capacity, size_t type_capacity, size_t user_data_stride);
Entity         *entity_create(uint32_t type);
void            entity_destroy(Entity *entity);
void            entity_release(Entity *entity);
void            entity_set_update_callback(uint32_t type, EntityUpdateFn callback);
void            entity_set_terminate_callback(uint32_t type, EntityTerminateFn callback);
void            entity_set_callback_context(uint32_t type, void *ctx);
void           *entity_get_user_data(Entity *entity);
void            entity_update(Entity *e);
void            entity_set_animation(Entity *e, Animation *animation);
void            entity_reset_animation(Entity *e);
void            entity_set_animation_speed(Entity *e, float speed);
AnimationFrame *entity_get_animation_frame(Entity *e);

void               physics_init(void);
void               physics_simulate(void);              
PhysicsBody       *physics_create_circle(Vec3 position, float radius, uint32_t type);
PhysicsBody       *physics_create_polygon_shape(Vec3 *positions, size_t count, uint32_t type);
void               physics_destroy_body(PhysicsBody *body);
PhysicsConstraint *physics_add_damped_spring(PhysicsBody *a, PhysicsBody *b, Vec2 anchor_a, Vec2 anchor_b, float rest_length, float stiffness, float damping);
PhysicsConstraint *physics_add_pin_joint(PhysicsBody *a, PhysicsBody *b, Vec2 anchor_a, Vec2 anchor_b, float dist);
PhysicsConstraint *physics_add_pivot_joint(PhysicsBody *a, PhysicsBody *b, Vec2 anchor_a, Vec2 anchor_b);
PhysicsConstraint *physics_add_rotary_limit_joint(PhysicsBody *a, PhysicsBody *b, float min, float max);
void               physics_set_filter(PhysicsBody *e, uint32_t group, uint32_t category, uint32_t mask);
void               physics_set_mass(PhysicsBody *e, float mass);
void               physics_set_force(PhysicsBody *e, Vec2 force);
void               physics_set_torque(PhysicsBody *e, float torque);
void               physics_set_angle(PhysicsBody *e, float angle);
void               physics_body_set_position(PhysicsBody *e, Vec2 pos);
Vec2               physics_body_get_position(PhysicsBody *e);
float              physics_body_get_angle(PhysicsBody *e);
Vec2               physics_get_center(PhysicsBody *e);

//**********************************************************
// KEK KEY 
//**********************************************************
void     tag_init(size_t capacity);
uint32_t tag_set(uint32_t tag, void *value);
void    *tag_get(uint32_t tag);
void     tag_remove(uint32_t tag);

//**********************************************************
// KEK HID 
//**********************************************************
void hid_init(size_t alias_capacity);
void hid_bind_alias_to_key(int alias, KeyboardKey key);
void hid_unbind_key(KeyboardKey key);
void hid_unbind_alias(int alias);
bool hid_is_alias_pressed(int alias);
bool hid_is_key_pressed(KeyboardKey key);
bool hid_is_mouse_button_pressed(MouseButton button);
Vec2 hid_get_mouse_position(void);

//**********************************************************
// KEK RNG 
//**********************************************************
void         random_init(void);
int          random_get_rangei(int min, int max);
unsigned int random_getui(void);
float        random_get_rangef(float min, float max);
Vec2         random_get_range_v2(Vec2 min, Vec2 max);
Vec3         random_get_range_v3(Vec3 min, Vec3 max);
Vec4         random_get_range_v4(Vec4 min, Vec4 max);

//**********************************************************
// KEK MATH 
//**********************************************************
float tween_calculate(int type, float t, float b, float c, float d);
float tween_linear(float t, float b, float c, float d);
float tween_ease_in_quad(float t, float b, float c, float d);
float tween_ease_out_quad(float t, float b, float c, float d);
float tween_ease_in_out_quad(float t, float b, float c, float d);
float tween_ease_in_cubic(float t, float b, float c, float d);
float tween_ease_out_cubic(float t, float b, float c, float d);
float tween_ease_in_out_cubic(float t, float b, float c, float d);
float tween_ease_in_quart(float t, float b, float c, float d);
float tween_ease_out_quart(float t, float b, float c, float d);
float tween_ease_in_out_quart(float t, float b, float c, float d);
float tween_ease_in_quint(float t, float b, float c, float d);
float tween_ease_out_quint(float t, float b, float c, float d);
float tween_ease_in_out_quint(float t, float b, float c, float d);
float tween_ease_in_sine(float t, float b, float c, float d);
float tween_ease_out_sine(float t, float b, float c, float d);
float tween_ease_in_out_sine(float t, float b, float c, float d);
float tween_ease_in_expo(float t, float b, float c, float d);
float tween_ease_out_expo(float t, float b, float c, float d);
float tween_ease_in_out_expo(float t, float b, float c, float d);
float tween_ease_in_circ(float t, float b, float c, float d);
float tween_ease_out_circ(float t, float b, float c, float d);
float tween_ease_in_out_circ(float t, float b, float c, float d);
float tween_ease_in_elastic(float t, float b, float c, float d);
float tween_ease_out_elastic(float t, float b, float c, float d);
float tween_ease_in_out_elastic(float t, float b, float c, float d);
float tween_ease_in_bounce(float t, float b, float c, float d);
float tween_ease_out_bounce(float t, float b, float c, float d);
float tween_ease_in_out_bounce(float t, float b, float c, float d);
float tween_ease_in_back(float t, float b, float c, float d);

//**********************************************************
// KEK MATH 
//**********************************************************
unsigned int aabb(Vec2 p0, Vec2 s0, Vec2 p1, Vec2 s1);
unsigned int aabb2(Rect2 a, Rect2 b);
unsigned int aabb3(Cube a, Cube b);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, Vec2 b);
Vec2 vec2_div(Vec2 a, Vec2 b);
Vec2 vec2_addf(Vec2 a, float b);
Vec2 vec2_subf(Vec2 a, float b);
Vec2 vec2_mulf(Vec2 a, float b);
Vec2 vec2_divf(Vec2 a, float b);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, Vec3 b);
Vec3 vec3_div(Vec3 a, Vec3 b);
Vec3 vec3_addf(Vec3 a, float b);
Vec3 vec3_subf(Vec3 a, float b);
Vec3 vec3_mulf(Vec3 a, float b);
Vec3 vec3_divf(Vec3 a, float b);
Vec4 vec4_add(Vec4 a, Vec4 b);
Vec4 vec4_sub(Vec4 a, Vec4 b);
Vec4 vec4_mul(Vec4 a, Vec4 b);
Vec4 vec4_div(Vec4 a, Vec4 b);
Vec4 vec4_addf(Vec4 a, float b);
Vec4 vec4_subf(Vec4 a, float b);
Vec4 vec4_mulf(Vec4 a, float b);
Vec4 vec4_divf(Vec4 a, float b);
Mat4 mat4_mul(Mat4 a, Mat4 b);
Vec4 mat4_mul_vec4(Mat4 m, Vec4 v);
Mat4 mat4_mulf(Mat4 m, float v);
Mat4 mat4_inverse(const Mat4 mat);
Mat4 mat4_transpose(Mat4 *m);
Mat4 mat4_translate(Vec3 pos);
Mat4 rotatex(float angle);
Mat4 rotatey(float angle);
Mat4 rotatez(float angle);
Mat4 mat4_scale(Vec3 scale);
Mat4 look_at(Vec3 eye, Vec3 target, Vec3 up);
Mat4 ortho_projection(float left, float right, float bottom, float top, float near, float far);
Mat4 perspective_projection(float fovy, float aspect, float near, float far);
Vec3 unproject(Vec3 pos, Mat4 viewproj, Vec4 viewport);
Vec3 screen_to_world_coord(Mat4 projection, Mat4 view, Vec4 viewport, Vec2 coord);
float clampf(float value, float min, float max);
float deg_to_rad(float deg);
float rad_to_deg(float rad);
int32_t line_intersect(Vec3 p0, Vec3 v0, Vec3 p1, Vec3 v1, Vec3 *c);
float vec2_dot_product(Vec2 a, Vec2 b);
float vec3_dot_product(Vec3 a, Vec3 b);
float vec4_dot_product(Vec4 a, Vec4 b);
Vec3 cross_product(Vec3 a, Vec3 b);
Vec2 vec2_project(Vec2 u, Vec2 v);
Vec3 vec3_project(Vec3 u, Vec3 v);
Vec4 vec4_project(Vec4 u, Vec4 v);
float vec2_length(Vec2 v);
float vec3_length(Vec3 v);
float vec4_length(Vec4 v);
float vec2_length2(Vec2 v);
float vec3_length2(Vec3 v);
float vec4_length2(Vec4 v);
Vec2 vec2_normal(Vec2 v);
Vec3 vec3_normal(Vec3 v);
Vec4 vec4_normal(Vec4 v);
Vec2 vec2_negative(Vec2 v);
Vec3 vec3_negative(Vec3 v);
Vec4 vec4_negative(Vec4 v);
int signi(int sign);
Vec2 vec2_sign(Vec2 v);
Vec3 vec3_sign(Vec3 v);
Vec4 vec4_sign(Vec4 v);
Vec2 vec2_abs(Vec2 v);
Vec3 vec3_abs(Vec3 v);
Vec4 vec4_abs(Vec4 v);
Vec2 vec2_clamp(Vec2 v, Vec2 min, Vec2 max);
Vec3 vec3_clamp(Vec3 v, Vec3 min, Vec3 max);
Vec4 vec4_clamp(Vec4 v, Vec4 min, Vec4 max);
Vec2 vec2_zero();
Vec3 vec3_zero();
Vec4 vec4_zero();
Mat4 mat4_zero();
Vec2 vec2_identity();
Vec3 vec3_identity();
Vec4 vec4_identity();
Mat4 mat4_identity();
Vec2 vec2(float x, float y);
Vec3 vec3(float x, float y, float z);
Vec4 vec4(float x, float y, float z, float w);
Vec2 vec3to2(Vec3 v);
Vec2 vec4to2(Vec4 v);
Vec3 vec2to3(Vec2 v);
Vec3 vec4to3(Vec4 v);
Vec4 vec2to4(Vec2 v);
Vec4 vec3to4(Vec3 v);
void vec3_order(Vec3 *p0, Vec3 *p1);
Vec2 line_line_intersect_point(Vec2 p0, Vec2 p1, Vec2 q0, Vec2 q1);
bool circle_circle_intersect(Vec2 p0, float r0, Vec2 p1, float r1);
float line_slope(Vec2 p0, Vec2 p1);
float line_normal_slope(Vec2 p0, Vec2 p1);
bool line_circle_collision(Vec2 p0, Vec2 p1, Vec2 c, float r, Vec2 *intersect);
bool point_line_bounce(Vec2 c0, Vec2 vel, Vec2 p0, Vec2 p1, Vec2 *p_intersect, Vec2 *c_out, Vec2 *dir);

