#include "kek.h"
#include "entity.h"
#include "create_tagged.h"
#include "tag.h"

static int window_width = 1920;
static int window_height = 1080;

typedef struct entity_data {
    int your_data;
}
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

    return KEK_OK;
}

int kek_user_update(void)
{
    framerate(60);

    if(is_hid_key_pressed(KEK_KBD_KEY_ESCAPE))
        quit_kek();

    return KEK_OK;
}

int kek_user_exit(void)
{
    return KEK_OK;
}

