#include "kek.h"
#include "kek_private.h"
#include "pal.h"
#include <stdio.h>

static int init_kek(int argc, char **argv);
static int run_kek(void);
static int terminate(void);
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
    init_kek(argc, argv);
    run_kek();
    terminate();
}

void framerate(int fps)
{
    framerate_fps = fps;
}
float get_target_frame_interval(void)
{
    return 1.0f/(float)framerate_fps;

}

static int init_kek(int argc, char **argv)
{
    framerate_fps = 60;
    log("initializing...");

    init_config();
    kek_user_pre_init(argc, argv);

    memstack_init(
        get_configi(KEK_CFG_MEM_STACK_CAPACITY));

    init_tag(
        get_configi(KEK_CFG_MEM_TAG_CAPACITY));

    init_spatialmap(
        get_configi(KEK_CFG_MEM_SPATIAL_MAP_CAPACITY));

    init_window();

    init_sound(
        get_configi(KEK_CFG_SOUND_SOURCE_CAPACITY),
        get_configi(KEK_CFG_SOUND_BUFFER_CAPACITY),
        get_configi(KEK_CFG_SOUND_CAPACITY));

    init_hid(
        get_configi(KEK_CFG_MEM_KEY_BIND_ALIAS_CAPACITY));

    init_camera(
        get_configi(KEK_CFG_MEM_CAMERA_CAPACITY));

    init_vertexbuffer(
        get_configi(KEK_CFG_MEM_VERTEX_BUFFER_CAPACITY));

    init_shader(
        get_configi(KEK_CFG_MEM_SHADER_CAPACITY));

    init_texture(
        get_configi(KEK_CFG_MEM_TEXTURE_CAPACITY));

    init_render(
        get_configi(KEK_CFG_MEM_RENDER_CAPACITY));

    init_scene(
        get_configi(KEK_CFG_MEM_SCENE_CAPACITY));

    init_animation(
        get_configi(KEK_CFG_MEM_ANIMATION_CAPACITY));

    init_physics();

    init_entity(
        get_configi(KEK_CFG_MEM_ENTITY_CAPACITY),
        get_configi(KEK_CFG_MEM_ENTITY_TYPE_CAPACITY),
        get_configi(KEK_CFG_MEM_ENTITY_USER_DATA_SIZE));

    init_random();

    return kek_user_enter(argc, argv);
}

void quit_kek(void)
{
    quit = true;
}

static int run_kek(void)
{
    log("running...");

    while(!quit)
    {
        float wait_until = pal_time() + (1.0f/(float)framerate_fps);

        poll_window();

        frame_time = pal_time();
        clearscreen();
        kek_user_update();
        int scene = get_active_scene();

        update_scene(scene);

        draw_scene(scene);

        swap_window_buffers();

        garbage_collect_scene(scene);
        while(pal_time() < wait_until) {}
    }

    return KEK_OK;
}

static int terminate(void)
{
    log("exiting...");
    
    int ret = kek_user_exit();

    memstack_free();

    return ret;
}
    
float get_frame_time(void)
{
    return frame_time;
}

