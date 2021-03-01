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
void     log_line(const char *file, int line, const char *function, const char *zone, const char *format, ...);
#define log(...) log_line(LOG_FILENAME,  __LINE__, __FUNCTION__, "INFO", __VA_ARGS__)
#define logd(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "DEBUG", __VA_ARGS__)
#define logw(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "WARN", __VA_ARGS__)
#define loge(...) log_line(LOG_FILENAME, __LINE__, __FUNCTION__, "ERROR", __VA_ARGS__)

//**********************************************************
// KEK MEMORY 
//**********************************************************
void  mempool_alloc(MemPool *pool, size_t capacity, size_t stride);
void *mempool_take(MemPool *pool);
int   mempool_get_slot(MemPool *pool, void *addr);
void *mempool_get_addr(MemPool *pool, int slot);
void  mempool_release(MemPool *pool, void *addr);
void  mempool_free(MemPool *pool);

void    memstack_init(size_t capacity);
void   *memstack_push(size_t size);
void    memstack_pop(void *addr);
void    memstack_free(void);
size_t  memstack_size(void);
size_t  memstack_capacity(void);

//**********************************************************
// KEK SPATIAL MAP 
//**********************************************************
#define WORLD_TO_SPATIAL_CELL(x) ((x) / 1024)
#define SPATIAL_CELL_TO_WORLD(x) ((x) * 1024)
void         init_spatialmap(size_t capacity);
size_t       get_spatialmap_count(void);
SpatialMap  *get_spatialmap(size_t index);
void         move_spatialmap_node(SpatialNode *node, int x, int y);
void         remove_spatialmap_node(SpatialNode *node);
void         add_spatialmap_node(SpatialNode *node, int x, int y);
void         query_spatialmap(int x0, int y0, int x1, int y1, SpatialMapQueryFn fn, void *ctx);
void         init_spatial_map(size_t capacity);
void         move_spatial_map_node(SpatialNode *node, int x, int y);
void         remove_spatial_map_node(SpatialNode *node);
void         add_spatial_map_node(SpatialNode *node, int x, int y);
void         query_spatial_map(int x0, int y0, int x1, int y1, SpatialMapQueryFn fn, void *ctx);


//**********************************************************
// KEK SOUND 
//**********************************************************
int   init_sound(size_t num_sources, size_t buffer_capacity, size_t sound_capacity);
void  update_sounds(void);
int   create_sound_buffer(void);
bool  load_wav_file_to_sound_buffer(int buffer, const char *file);
int   master_volume(float vol);
int   create_sound(int buffer);
void  play_sound(int sound);
void  destroy_sound(int sound);
void  pause_sound(int sound);
void  stop_sound(int sound);
void  sound_loop(int sound, bool loop);
void  sound_pitch(int sound, float value);
void  sound_gain(int sound, float value);
float get_sound_pitch(int sound);
float get_sound_gain(int sound);
bool  get_sound_loop(int sound);
bool  is_sound_playing(int sound);

//**********************************************************
// KEK VERTEX BUFFER
//**********************************************************
void          init_vertexbuffer(size_t capacity);
int           create_vertexbuffer(size_t capacity);
VertexBuffer *get_vertexbuffer(int vid);
void          destroy_vertexbuffer(int vbid);
void          vertexbuffer_attribs(int vbid, size_t *attribs, size_t count);
size_t        get_vertexbuffer_capacity(int vbid);
size_t        get_vertexbuffer_size(int vbid);
void          clear_vertexbuffer(int vbid);
int           fill_vertexbuffer(int vbid, uint8_t *data, size_t size);
void          bind_vertexbuffer(int vbid);
void          draw_vertexbuffer(int vbid, size_t start, size_t count);
void          draw_vertexbuffer_line_strip(int vbid, size_t start, size_t count);
void          draw_vertexbuffer_lines(int vbid, size_t start, size_t count);
int           map_vertexbuffer(int vbid);
int           unmap_vertexbuffer(int vbid);
void          append_vertexbuffer(int vbid, uint8_t *data, size_t vertices);

//**********************************************************
// KEK SHADER
//**********************************************************
void    init_shader(size_t capacity);
int     create_shader(void);
Shader *get_shader(int shaderid);
void    destroy_shder(int shaderid);
int     load_shader_files(int shaderid, const char *vert_file, const char *frag_file);
int     load_shader_buffer(int shaderid, const char *vert_buffer, const char *frag_buffer);
int     bind_shader(int shaderid);

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
int      create_texture(void);
Texture *get_texture(int texture_id);
void     destroy_texture(int texture_id);
int      load_texture_file(int texture_id, const char *file);
void     bind_texture(int texture_id, int slot);

//**********************************************************
// KEK ANIMATION
//**********************************************************
void       init_animation(size_t capacity);
int        create_animation(void);
Animation *get_animation(int animationid);
void       destroy_animation(int animationid);
void       add_animation_frame(int animationid, AnimationFrame frame);
void       add_animation_frame_clip(int animationid, int textureid, int x, int y, int clip_width, int clip_height, float duration);
void       animation_loop(int animationid, bool loop);

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
Render *create_entity_render(void);
Render *create_entity_box_render(void);
Render *create_circle_render(void);
Render *create_rect_render(void);
void    destroy_render(Render *render);
void    draw_render_lines_default(Render *render, int camera, Vec3 *points, size_t count, void *ctx);
void    draw_render_entities(Render *render, int camera, int *entities, size_t count, void *ctx);
void    draw_render_entity_boxes(Render *render, int camera, int *entities, size_t count, void *ctx);
void    draw_render_collision_boxes(Render *render, int camera, int *entities, size_t count, void *ctx);
void    draw_render_collision_circles(Render *render, int camera, int *entities, size_t count, void *ctx);
void    draw_render_spatialmap(Render *render, int camera, int *entities, size_t count);
//
//**********************************************************
// KEK CAMERA 
//**********************************************************
void    init_camera(size_t capacity);
Camera *get_camera(int cameraid);
int     create_camera(void);
void    destroy_camera(int cameraid);
void    get_camera_ortho_mvp(int cameraid, Mat4 *mvp);
Vec2    get_camera_mouse_position(int cameraid);
Vec3    get_camera_position(int cameraid);
void    update_camera(int cameraid);
void    camera_offset(int cameraid, Vec3 offset);
void    camera_ortho_zoom(int cameraid, float zoom);
void    camera_position(int cameraid, Vec3 position);
void    camera_update_cb(int cameraid, CameraUpdateFn update_cb, void *ctx);

//**********************************************************
// KEK SCENE 
//**********************************************************
void    init_scene(size_t capacity);
int     create_scene(void);
Scene  *get_scene(int sceneid);
void    destroy_scene(int sceneid);
void    scene_camera(int sceneid, int camera);
int     get_scene_camera(int sceneid);
void    scene_active(int sceneid);
int     get_active_scene(void);
void    update_scene(int sceneid);
void    draw_scene(int sceneid);
void    add_scene_entity(int sceneid, int entityid);
void    garbage_collect_scene(int sceneid);
void    query_scene_entities(int sceneid, Vec2 pos, Vec2 size, SceneQueryEntityFn fn, void *ctx);

//**********************************************************
// KEK ENTITY
//**********************************************************
void            init_entity(size_t capacity, size_t type_capacity, size_t user_data_stride);
int             create_entity(uint32_t type);
void            destroy_entity(int entityid);
void            release_entity(int entityid);
void            entity_update_callback(uint32_t type, EntityUpdateFn callback);
void            entity_terminate_callback(uint32_t type, EntityTerminateFn callback);
void            entity_callback_context(uint32_t type, void *ctx);
uint32_t        get_entity_render_key(int entityid);
Entity         *get_entity(int entityid);
Collider       *get_entity_collider(int entityid);
void           *get_entity_user_data(int entityid);
void            update_entity(int entityid);
int             get_entity_type(int entityid);
void            entity_type(int entityid, int type);
Vec3            get_entity_size(int entityid);
void            entity_size(int entityid, Vec3 size);
Vec3            get_entity_position(int entityid);
void            entity_position(int entityid, Vec3 position);
Vec3            get_entity_velocity(int entityid);
void            entity_velocity(int entityid, Vec3 velocity);
Vec4            get_entity_colormask(int entityid);
void            entity_colormask(int entityid, Vec4 velocity);
int             get_entity_texture(int entityid);
void            entity_texture(int entityid, int texture);
int             get_entity_animation(int entityid);
void            entity_animation(int entityid, int animationid);

Vec3            get_entity_rotation(int entityid);
void            entity_rotation(int entityid, Vec3 rotation);
float           get_entity_rotation_z(int entityid);
void            entity_rotation_z(int entityid, float rotation);

void            entity_gravity_scale(int entityid, float scale);
float           get_entity_gravity_scale(int entityid);

Vec3            get_entity_texture_rotation(int entityid);
void            entity_texture_rotation(int entityid, Vec3 rotation);
float           get_entity_texture_rotation_z(int entityid);
void            entity_texture_rotation_z(int entityid, float rotation);

void            reset_entity_animation(int entityid);
void            set_entity_animation_speed(int entityid, float speed);
AnimationFrame *get_entity_animation_frame(int entityid);
void            query_entity(Vec2 p0, Vec2 p1, EntityQueryFn fn, void *ctx);
void            entity_circle_collider(int entityid, float radius);
void            entity_collider_mask(int entityid, uint32_t mask);

//**********************************************************
// KEK PHYSICS 
//**********************************************************
void init_physics(void);
void simulate_physics(int scene);
void gravity(Vec3 gravity);

//**********************************************************
// KEK KEY 
//**********************************************************
void     init_tag(size_t capacity);
void     set_tag(uint32_t tag, void *value);
void     set_tagi(uint32_t tag, int value);
void    *get_tag(uint32_t tag);
int      get_tagi(uint32_t tag);
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

unsigned int aabb(Vec2 p0, Vec2 s0, Vec2 p1, Vec2 s1);
unsigned int aabb2(Rect a, Rect b);
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

