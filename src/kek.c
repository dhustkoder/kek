#include "kek.h"
#include "kek_private.h"
#include "pal.h"
#include <stdio.h>

static int kek_init(int argc, char **argv);
static int kek_run(void);
static int kek_terminate(void);
static void kek_print_screen_size(void);

extern int kek_user_pre_init(int argc, char **argv);
extern int kek_user_enter(int argc, char **argv);
extern int kek_user_update(void);
extern int kek_user_exit(void);

static bool quit = false;
static int framerate_fps = 0;
static float frame_time = 0.0f;

void pal_main(int argc, char **argv)
{
    kek_init(argc, argv);
    kek_run();
    kek_terminate();
}

void kek_set_framerate(int fps)
{
    framerate_fps = fps;
}
float kek_get_target_frame_interval(void)
{
    return 1.0f/(float)framerate_fps;

}

static int kek_init(int argc, char **argv)
{
    PALReturn pal_ret;

    framerate_fps = 60;
    log("initializing...");

    config_init();
    kek_user_pre_init(argc, argv);

    size_t capacity;

    mem_stack_init(
        config_geti(KEK_CFG_MEM_STACK_CAPACITY));

    tag_init(
        config_geti(KEK_CFG_MEM_TAG_CAPACITY));

    qtree_init(
        config_geti(KEK_CFG_MEM_QTREE_CAPACITY));

    window_init();

    hid_init(
        config_geti(KEK_CFG_MEM_KEY_BIND_ALIAS_CAPACITY));

    camera_init(
        config_geti(KEK_CFG_MEM_CAMERA_CAPACITY));

    vertex_buffer_init(
        config_geti(KEK_CFG_MEM_VERTEX_BUFFER_CAPACITY));

    shader_init(
        config_geti(KEK_CFG_MEM_SHADER_CAPACITY));

    texture_init(
        config_geti(KEK_CFG_MEM_TEXTURE_CAPACITY));

    render_init(
        config_geti(KEK_CFG_MEM_RENDER_CAPACITY));

    scene_init(
        config_geti(KEK_CFG_MEM_SCENE_CAPACITY));

    animation_init(
        config_geti(KEK_CFG_MEM_ANIMATION_CAPACITY));

    physics_init();

    entity_init(
        config_geti(KEK_CFG_MEM_ENTITY_CAPACITY),
        config_geti(KEK_CFG_MEM_ENTITY_TYPE_CAPACITY),
        config_geti(KEK_CFG_MEM_ENTITY_USER_DATA_SIZE));

    random_init();

    return kek_user_enter(argc, argv);
}

void kek_quit(void)
{
    quit = true;
}

void entity_print_qtree(void);
static int kek_run(void)
{
    log("running...");

    while(!quit)
    {
        float wait_until = pal_time_live() + (1.0f/(float)framerate_fps);

        window_poll();

        frame_time = pal_time_live();
        window_clearscreen();
        kek_user_update();
        Scene *scene = scene_get_active();

        scene_update(scene);

        entity_print_qtree();
        scene_draw(scene);

        window_swap_buffers();

        scene_garbage_collect(scene);
        while(pal_time_live() < wait_until) {}
    }

    return KEK_OK;
}

static int kek_terminate(void)
{
    log("exiting...");
    
    int ret = kek_user_exit();

    mem_stack_free();

    return ret;
}
    
float kek_get_frame_time(void)
{
    return frame_time;
}

