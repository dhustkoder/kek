#include "kek.h"

static struct mem_pool pool;

void camera_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct camera));
}

struct camera *camera_create(void)
{
    struct camera *camera = mem_pool_take(&pool);
    camera->zoom = 1.0f;
    camera->position = vec3_zero();

    return camera;
}

void camera_destroy(struct camera *camera)
{
    mem_pool_release(&pool, camera);
}

void camera_set_ortho_zoom(struct camera *camera, float zoom)
{
    camera->zoom = zoom;
}

void camera_set_position(struct camera *camera, union vec3 position)
{
    camera->position = position;
}

void camera_get_ortho_mvp(struct camera *camera, struct mat4 *mvp)
{
    union vec3 position = camera->position;
    float zoom = camera->zoom;

    unsigned int window_width = 0;
    unsigned int window_height = 0;

    window_get_size(&window_width, &window_height);

    struct mat4 model      = mat4_identity();
    struct mat4 view       = mat4_inverse(mat4_translate(position));
    struct mat4 projection = ortho_projection(
                               -(float)window_width  / (zoom*2.0f), 
                                (float)window_width  / (zoom*2.0f), 
                               -(float)window_height / (zoom*2.0f), 
                                (float)window_height / (zoom*2.0f), 
                                0.0f, 1.f); 		

	*mvp = mat4_identity();
	*mvp = mat4_mul(*mvp, projection);
	*mvp = mat4_mul(*mvp, view);
	*mvp = mat4_mul(*mvp, model);
}

union vec2 camera_get_mouse_position(struct camera *camera)
{
    union vec2 mousepos = hid_get_mouse_position();
    union vec2 pos = camera->position.xy;

    unsigned int window_width = 0;
    unsigned int window_height = 0;
    float zoom = camera->zoom;

    window_get_size(&window_width, &window_height);

    // make mouse position relative to center
    mousepos.x -=  window_width / 2.f;
    mousepos.y -=  window_height / 2.f;

    pos.x += mousepos.x / zoom; 
    pos.y += mousepos.y / zoom; 
    return pos;
}
