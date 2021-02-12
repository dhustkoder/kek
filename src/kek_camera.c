#include "kek.h"

static MemPool pool;

void init_camera(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Camera));
}

Camera *create_camera(void)
{
    Camera *camera = mem_pool_take(&pool);
    camera->zoom = 1.0f;
    camera->position = zero_vec3();

    return camera;
}

void destroy_camera(Camera *camera)
{
    mem_pool_release(&pool, camera);
}

void camera_ortho_zoom(Camera *camera, float zoom)
{
    camera->zoom = zoom;
}

void camera_position(Camera *camera, Vec3 position)
{
    camera->position = position;
}

void get_camera_ortho_mvp(Camera *camera, Mat4 *mvp)
{
    Vec3 position = camera->position;
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

Vec2 get_camera_mouse_position(Camera *camera)
{
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

