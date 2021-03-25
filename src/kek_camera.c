#include "kek.h"

static MemPool pool;

void init_camera(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Camera));
}

int create_camera(void)
{
    Camera *camera = mempool_take(&pool);
    camera->id = mempool_get_slot(&pool, camera);
    camera->zoom = 1.0f;
    camera->position = zero_vec3();
    camera->offset = zero_vec3();
    camera->update_cb = NULL;
    camera->update_ctx = NULL;

    return camera->id;
}

Camera *get_camera(int cameraid)
{
    return mempool_get_addr(&pool, cameraid);
}

void destroy_camera(int cameraid)
{
    Camera *camera = get_camera(cameraid);
    mempool_release(&pool, camera);
}

void camera_ortho_zoom(int cameraid, float zoom)
{
    Camera *camera = get_camera(cameraid);
    camera->zoom = zoom;
}

void camera_offset(int cameraid, Vec3 offset)
{
    Camera *camera = get_camera(cameraid);
    camera->offset = offset;
}

void update_camera(int cameraid)
{
    Camera *camera = get_camera(cameraid);
    if(camera->update_cb)
        camera->update_cb(cameraid, camera->update_ctx);
}

void camera_update_cb(int cameraid, CameraUpdateFn update_cb, void *ctx)
{
    Camera *camera = get_camera(cameraid);
    camera->update_cb = update_cb;
    camera->update_ctx = ctx;
}

void camera_position(int cameraid, Vec3 position)
{
    Camera *camera = get_camera(cameraid);
    camera->position = position;
}

Vec3 get_camera_position(int cameraid)
{
    Camera *camera = get_camera(cameraid);
    return camera->position;
}

float get_camera_zoom(int cameraid)
{
    Camera *camera = get_camera(cameraid);
    return camera->zoom;
}

void get_camera_ortho_mvp(int cameraid, Mat4 *mvp)
{
    Camera *camera = get_camera(cameraid);
    Vec3 position = add_vec3(camera->position, camera->offset);
    float zoom = camera->zoom;

    unsigned int window_width = 0;
    unsigned int window_height = 0;

    get_window_size(&window_width, &window_height);

    Mat4 model      = identity_mat4();
    Mat4 view       = inverse_mat4(translate_mat4(position));
    Mat4 projection = ortho_projection(
                               -(float)window_width  / (zoom*2.0f), 
                                (float)window_width  / (zoom*2.0f), 
                               -(float)window_height / (zoom*2.0f), 
                                (float)window_height / (zoom*2.0f), 
                                0.0f, 1.f); 		

	*mvp = identity_mat4();
	*mvp = mul_mat4(*mvp, projection);
	*mvp = mul_mat4(*mvp, view);
	*mvp = mul_mat4(*mvp, model);
}

Vec2 get_camera_mouse_position(int cameraid)
{
    Camera *camera = get_camera(cameraid);
    Vec2 mousepos = get_mouse_position();
    Vec2 pos = camera->position.xy;

    unsigned int window_width = 0;
    unsigned int window_height = 0;
    float zoom = camera->zoom;

    get_window_size(&window_width, &window_height);

    // make mouse position relative to center
    mousepos.x -=  window_width / 2.f;
    mousepos.y -=  window_height / 2.f;

    pos.x += mousepos.x / zoom; 
    pos.y += mousepos.y / zoom; 
    return pos;
}

