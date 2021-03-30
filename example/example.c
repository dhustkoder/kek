#include "kek.h"

static int window_width = 640;
static int window_height = 360; 
static int scene_id;

typedef struct entity_data {
    int your_data;
} EntityData;

int kek_user_pre_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    configi(KEK_CFG_MEM_ENTITY_USER_DATA_SIZE, sizeof(EntityData));

    return KEK_OK;
}

int kek_user_enter(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    log("Entering application");

    // window configuration
    open_window("Example app", window_width, window_height); 
    master_volume(0.3f);

    window_title("Example1");
    window_size(window_width, window_height);

    clearscreen_color(vec4(1,1,1,1));
    scene_id = create_scene();
    scene_active(scene_id);

    return KEK_OK;
}

int kek_user_update(void)
{
    log("kek update");

    framerate(60);

    if (is_hid_key_pressed(KEK_KBD_KEY_ESCAPE))
        quit_kek();

    return KEK_OK;
}

int kek_user_exit(void)
{
    destroy_scene(scene_id);
    return KEK_OK;
}

