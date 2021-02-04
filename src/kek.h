#pragma once
#include "kek_type.h"
#include "glad/gl.h"
#include <stdarg.h>
#include <string.h>

//**********************************************************
// KEK GENERAL 
//**********************************************************
void kek_quit(void);
void kek_set_framerate(int fps);
float kek_get_frame_time(void);
float kek_get_target_frame_interval(void);

//**********************************************************
// KEK CONFIG
//**********************************************************
void config_init(void);
void config_seti(enum config_type type, int value);
int  config_geti(enum config_type type);

//**********************************************************
// KEK WINDOW 
//**********************************************************
int window_init(void);
int  window_set_title(const char *title);
int  window_set_size(unsigned int width, unsigned int height);
int  window_get_size(unsigned int *width, unsigned int *height);
int  window_open(const char *title, unsigned int width, unsigned int height);
struct pal_window *window_get_pal(void);
void       window_clearscreen(void);
void       window_set_clearscreen_color(union vec4 color);

//**********************************************************
// KEK LOG 
//**********************************************************
#define LOG_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
int log_line(const char *file, int line, const char *function, const char *zone, const char *format, ...);
#define log(...) log_line(LOG_FILENAME,  __LINE__, __FUNCTION__, "INFO", __VA_ARGS__)
#define logd(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "DEBUG", __VA_ARGS__)
#define logw(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "WARN", __VA_ARGS__)
#define loge(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "ERROR", __VA_ARGS__)

//**********************************************************
// KEK MEMORY 
//**********************************************************
void  mem_pool_alloc(struct mem_pool *pool, size_t capacity, size_t stride);
void *mem_pool_take(struct mem_pool *pool);
void *mem_pool_release(struct mem_pool *pool, void *addr);
void  mem_pool_free(struct mem_pool *pool);

void  mem_stack_init(size_t capacity);
void *mem_stack_push(size_t size);
void  mem_stack_pop(void *addr);
void  mem_stack_free(void);

//**********************************************************
// KEK VERTEX BUFFER
//**********************************************************
void             
vertex_buffer_init(size_t capacity);

struct vertex_buffer *vertex_buffer_create(size_t capacity);
void             vertex_buffer_destroy(struct vertex_buffer *vb);
void             vertex_buffer_set_attribs(struct vertex_buffer *vb, size_t *attribs, size_t count);
size_t           vertex_buffer_get_capacity(struct vertex_buffer *vb);
size_t           vertex_buffer_get_size(struct vertex_buffer *vb);
void             vertex_buffer_clear(struct vertex_buffer *vb);
int        vertex_buffer_fill(struct vertex_buffer *vb, uint8_t *data, size_t size);
void             vertex_buffer_bind(struct vertex_buffer *vb);
void             vertex_buffer_draw(struct vertex_buffer *vb, size_t start, size_t count);
int        vertex_buffer_map(struct vertex_buffer *vb);
int        vertex_buffer_unmap(struct vertex_buffer *vb);
int        vertex_buffer_append(struct vertex_buffer *vb, uint8_t *data, size_t vertices);

//**********************************************************
// KEK SHADER
//**********************************************************
void       shader_init(size_t capacity);
struct shader *shader_create(void);
void       shader_destroy(struct shader *shader);
int  shader_load_files(struct shader *shader, const char *vert_file, const char *frag_file);
int  shader_load_buffer(struct shader *shader, const char *vert_buffer, const char *frag_buffer);
int  shader_free(struct shader *shader);
int  shader_bind(struct shader *shader);

//**********************************************************
// KEK TEXTURE 
//**********************************************************
void        texture_init(size_t capacity);
struct texture *texture_create(void);
void        texture_destroy(struct texture *texture);
int   texture_load_file(struct texture *texture, const char *file);
void        texture_bind(struct texture *textures, int slot);

//**********************************************************
// KEK ANIMATION
//**********************************************************
void               animation_init(size_t capacity);
struct animation      *animation_create(void);
void               animation_destroy(struct animation *animation);
void               animation_add_frame(struct animation *animation, struct animation_frame frame);
void               animation_set_loop(struct animation *animation, bool loop);

//**********************************************************
// KEK RENDER
//**********************************************************
void       render_init(size_t capacity);
struct render *render_create(void);
void       render_destroy(struct render *render);
void       render_set_draw_callback(struct render *render, KEKRenderFn fn, void *ctx);
void       render_draw(struct render *render, struct camera *camera, struct entity *entity);
//
//**********************************************************
// KEK CAMERA 
//**********************************************************
void       camera_init(size_t capacity);
struct camera *camera_create(void);
void       camera_destroy(struct camera *camera);
void       camera_set_ortho_zoom(struct camera *camera, float zoom);
void       camera_get_ortho_mvp(struct camera *camera, struct mat4 *mvp);
void       camera_set_position(struct camera *camera, union vec3 position);
union vec2    camera_get_mouse_position(struct camera *camera);

//**********************************************************
// KEK SCENE 
//**********************************************************
void       scene_init(size_t capacity);
struct scene  *scene_create(void);
void       scene_destroy(struct scene *scene);
void       scene_set_camera(struct scene *scene, struct camera *camera);
struct camera *scene_get_camera(struct scene *scene);
void       scene_set_active(struct scene *scene);
struct scene  *scene_get_active(void);
void       scene_update(struct scene *scene);
void       scene_draw(struct scene *scene);
void       scene_add_entity(struct scene *scene, struct entity *entity);
void       scene_garbage_collect(struct scene *scene);
void       scene_query_entities_aabb(struct scene *scene, union vec2 pos, union vec2 size, KEKSceneQueryEntityFn fn, void *ctx);

//**********************************************************
// KEK ENTITY
//**********************************************************
void               entity_init(size_t capacity, size_t type_capacity, size_t user_data_stride);
struct entity         *entity_create(uint32_t type);
void               entity_destroy(struct entity *entity);
void               entity_release(struct entity *entity);
void               entity_set_init_callback(uint32_t type, KEKEntityInitFn callback);
void               entity_set_update_callback(uint32_t type, KEKEntityUpdateFn callback);
void               entity_set_terminate_callback(uint32_t type, KEKEntityTerminateFn callback);
void               entity_set_callback_context(uint32_t type, void *ctx);
void              *entity_get_user_data(struct entity *entity);
void               entity_update(struct entity *e);
void               entity_set_animation(struct entity *e, struct animation *animation);
void               entity_reset_animation(struct entity *e);
void               entity_set_animation_speed(struct entity *e, float speed);
struct animation_frame *entity_get_animation_frame(struct entity *e);

void                  physics_init(void);
void                  physics_simulate(void);              
struct physics_body       *physics_create_circle(union vec3 position, float radius, uint32_t type);
struct physics_body       *physics_create_polygon_shape(union vec3 *positions, size_t count, uint32_t type);
void                  physics_destroy_body(struct physics_body *body);
struct physics_constraint *physics_add_damped_spring(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b, float rest_length, float stiffness, float damping);
struct physics_constraint *physics_add_pin_joint(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b, float dist);
struct physics_constraint *physics_add_pivot_joint(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b);
struct physics_constraint *physics_add_rotary_limit_joint(struct physics_body *a, struct physics_body *b, float min, float max);
void                  physics_set_filter(struct physics_body *e, uint32_t group, uint32_t category, uint32_t mask);
void                  physics_set_mass(struct physics_body *e, float mass);
void                  physics_set_force(struct physics_body *e, union vec2 force);
void                  physics_set_torque(struct physics_body *e, float torque);
void                  physics_set_angle(struct physics_body *e, float angle);
void                  physics_body_set_position(struct physics_body *e, union vec2 pos);
union vec2               physics_body_get_position(struct physics_body *e);
float                 physics_body_get_angle(struct physics_body *e);
union vec2               physics_get_center(struct physics_body *e);

//**********************************************************
// KEK KEY 
//**********************************************************
void    tag_init(size_t capacity);
uint32_t  tag_set(uint32_t tag, void *value);
void   *tag_get(uint32_t tag);
void    tag_remove(uint32_t tag);

//**********************************************************
// KEK HID 
//**********************************************************
void hid_init(size_t alias_capacity);
void hid_bind_alias_to_key(int alias, enum keyboard_key key);
void hid_unbind_key(enum keyboard_key key);
void hid_unbind_alias(int alias);
bool hid_is_alias_pressed(int alias);
bool hid_is_key_pressed(enum keyboard_key key);
bool hid_is_mouse_button_pressed(enum mouse_button button);
union vec2 hid_get_mouse_position(void);

//**********************************************************
// KEK RNG 
//**********************************************************
void random_init(void);
int random_get_rangei(int min, int max);
unsigned int random_getui(void);
float random_get_rangef(float min, float max);
union vec2 random_get_range_v2(union vec2 min, union vec2 max);
union vec3 random_get_range_v3(union vec3 min, union vec3 max);
union vec4 random_get_range_v4(union vec4 min, union vec4 max);

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
unsigned int aabb(union vec2 p0, union vec2 s0, union vec2 p1, union vec2 s1);
unsigned int aabb2(struct rect2 a, struct rect2 b);
unsigned int aabb3(struct cube a, struct cube b);
union vec2 vec2_add(union vec2 a, union vec2 b);
union vec2 vec2_sub(union vec2 a, union vec2 b);
union vec2 vec2_mul(union vec2 a, union vec2 b);
union vec2 vec2_div(union vec2 a, union vec2 b);
union vec2 vec2_addf(union vec2 a, float b);
union vec2 vec2_subf(union vec2 a, float b);
union vec2 vec2_mulf(union vec2 a, float b);
union vec2 vec2_divf(union vec2 a, float b);
union vec3 vec3_add(union vec3 a, union vec3 b);
union vec3 vec3_sub(union vec3 a, union vec3 b);
union vec3 vec3_mul(union vec3 a, union vec3 b);
union vec3 vec3_div(union vec3 a, union vec3 b);
union vec3 vec3_addf(union vec3 a, float b);
union vec3 vec3_subf(union vec3 a, float b);
union vec3 vec3_mulf(union vec3 a, float b);
union vec3 vec3_divf(union vec3 a, float b);
union vec4 vec4_add(union vec4 a, union vec4 b);
union vec4 vec4_sub(union vec4 a, union vec4 b);
union vec4 vec4_mul(union vec4 a, union vec4 b);
union vec4 vec4_div(union vec4 a, union vec4 b);
union vec4 vec4_addf(union vec4 a, float b);
union vec4 vec4_subf(union vec4 a, float b);
union vec4 vec4_mulf(union vec4 a, float b);
union vec4 vec4_divf(union vec4 a, float b);
struct mat4 mat4_mul(struct mat4 a, struct mat4 b);
union vec4 mat4_mul_vec4(struct mat4 m, union vec4 v);
struct mat4 mat4_mulf(struct mat4 m, float v);
struct mat4 mat4_inverse(const struct mat4 mat);
struct mat4 mat4_transpose(struct mat4 *m);
struct mat4 mat4_translate(union vec3 pos);
struct mat4 rotatex(float angle);
struct mat4 rotatey(float angle);
struct mat4 rotatez(float angle);
struct mat4 mat4_scale(union vec3 scale);
struct mat4 look_at(union vec3 eye, union vec3 target, union vec3 up);
struct mat4 ortho_projection(float left, float right, float bottom, float top, float near, float far);
struct mat4 perspective_projection(float fovy, float aspect, float near, float far);
union vec3 unproject(union vec3 pos, struct mat4 viewproj, union vec4 viewport);
union vec3 screen_to_world_coord(struct mat4 projection, struct mat4 view, union vec4 viewport, union vec2 coord);
float clampf(float value, float min, float max);
float deg_to_rad(float deg);
float rad_to_deg(float rad);
int32_t line_intersect(union vec3 p0, union vec3 v0, union vec3 p1, union vec3 v1, union vec3 *c);
float vec2_dot_product(union vec2 a, union vec2 b);
float vec3_dot_product(union vec3 a, union vec3 b);
float vec4_dot_product(union vec4 a, union vec4 b);
union vec3 cross_product(union vec3 a, union vec3 b);
union vec2 vec2_project(union vec2 u, union vec2 v);
union vec3 vec3_project(union vec3 u, union vec3 v);
union vec4 vec4_project(union vec4 u, union vec4 v);
float vec2_length(union vec2 v);
float vec3_length(union vec3 v);
float vec4_length(union vec4 v);
float vec2_length2(union vec2 v);
float vec3_length2(union vec3 v);
float vec4_length2(union vec4 v);
union vec2 vec2_normal(union vec2 v);
union vec3 vec3_normal(union vec3 v);
union vec4 vec4_normal(union vec4 v);
union vec2 vec2_negative(union vec2 v);
union vec3 vec3_negative(union vec3 v);
union vec4 vec4_negative(union vec4 v);
int signi(int sign);
union vec2 vec2_sign(union vec2 v);
union vec3 vec3_sign(union vec3 v);
union vec4 vec4_sign(union vec4 v);
union vec2 vec2_abs(union vec2 v);
union vec3 vec3_abs(union vec3 v);
union vec4 vec4_abs(union vec4 v);
union vec2 vec2_clamp(union vec2 v, union vec2 min, union vec2 max);
union vec3 vec3_clamp(union vec3 v, union vec3 min, union vec3 max);
union vec4 vec4_clamp(union vec4 v, union vec4 min, union vec4 max);
union vec2 vec2_zero();
union vec3 vec3_zero();
union vec4 vec4_zero();
struct mat4 mat4_zero();
union vec2 vec2_identity();
union vec3 vec3_identity();
union vec4 vec4_identity();
struct mat4 mat4_identity();
union vec2 vec2(float x, float y);
union vec3 vec3(float x, float y, float z);
union vec4 vec4(float x, float y, float z, float w);
union vec2 vec3to2(union vec3 v);
union vec2 vec4to2(union vec4 v);
union vec3 vec2to3(union vec2 v);
union vec3 vec4to3(union vec4 v);
union vec4 vec2to4(union vec2 v);
union vec4 vec3to4(union vec3 v);
void vec3_order(union vec3 *p0, union vec3 *p1);
union vec2 line_line_intersect_point(union vec2 p0, union vec2 p1, union vec2 q0, union vec2 q1);
bool circle_circle_intersect(union vec2 p0, float r0, union vec2 p1, float r1);
float line_slope(union vec2 p0, union vec2 p1);
float line_normal_slope(union vec2 p0, union vec2 p1);
bool line_circle_collision(union vec2 p0, union vec2 p1, union vec2 c, float r, union vec2 *intersect);
bool point_line_bounce(union vec2 c0, union vec2 vel, union vec2 p0, union vec2 p1, union vec2 *p_intersect, union vec2 *c_out, union vec2 *dir);

