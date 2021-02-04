#include "kek.h"

static int config[KEK_CFG_NUM_TYPES];

void config_init(void)
{
    for(size_t i = 0; i < KEK_CFG_NUM_TYPES; ++i)
        config[i] = 0;

    config[KEK_CFG_MEM_STACK_CAPACITY]                 = 1024 * 1024;
    config[KEK_CFG_MEM_TAG_CAPACITY]                   = 1024;
    config[KEK_CFG_MEM_CAMERA_CAPACITY]                = 1024;
    config[KEK_CFG_MEM_VERTEX_BUFFER_CAPACITY]         = 1024;
    config[KEK_CFG_MEM_SHADER_CAPACITY]                = 1024;
    config[KEK_CFG_MEM_ENTITY_CAPACITY]                = 16384;
    config[KEK_CFG_MEM_ENTITY_TYPE_CAPACITY]           = 1024;
    config[KEK_CFG_MEM_ENTITY_USER_DATA_SIZE]          = 0;
    config[KEK_CFG_MEM_SCENE_CAPACITY]                 = 1024;
    config[KEK_CFG_MEM_TEXTURE_CAPACITY]               = 512;
    config[KEK_CFG_MEM_RENDER_CAPACITY]                = 128;
    config[KEK_CFG_MEM_KEY_BIND_ALIAS_CAPACITY]        = 256;
    config[KEK_CFG_MEM_ANIMATION_CAPACITY]             = 1024;
}

void config_seti(enum config_type type, int value)
{
    config[type] = value;
}

int config_geti(enum config_type type)
{
    return config[type];
}

