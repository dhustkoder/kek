#pragma once

#include "kek_gl.h"
#include "pal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>


#define KUT_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define KUT_STRUCT_ATTRIB_SIZE(type, member) sizeof(((type *)0)->member)

#define KUT_PI M_PI
#define KUT_PI_2 (PJ_PI * 0.5f)
#define KUT_MIN(a, b) (((a) < (b)) ? (a) : (b)) 
#define KUT_MAX(a, b) (((a) > (b)) ? (a) : (b)) 

// forward declarations
struct entity;
struct camera;
struct render;

typedef void (*KEKEntityInitFn)(struct entity *entity, void *ctx); 
typedef void (*KEKEntityTerminateFn)(struct entity *entity, void *ctx); 
typedef void (*KEKEntityUpdateFn)(struct entity *entity, void *ctx); 
typedef void (*KEKEntityQueryFn)(struct entity *e, void *ctx);
typedef void (*KEKCollisionFn)(struct entity *a, struct entity *b, void *ctx);
typedef void (*KEKRenderFn)(struct render *render, struct camera *camera, struct entity *entity, void *ctx);
typedef void (*KEKSceneQueryEntityFn)(struct entity *entity, void *ctx);

enum {
    KEK_OK,
    KEK_ERROR,
    KEK_OUT_OF_MEMORY,
    KEK_NOT_INITIALIZED,
    KEK_INIT_ERROR,
    KEK_BAD_PARAM,
    KEK_ALREADY_FREE,
    KEK_UNKNOWN_ERROR,
    KEK_NOT_IMPLEMENTED,
    KEK_WINDOW_OPEN_FAIL,
    KEK_WINDOW_ALREADY_OPEN,
    KEK_WINDOW_NOT_OPEN,
    KEK_WINDOW_SWAP_BUFFER_ERROR,
    KEK_WINDOW_POLL_ERROR,
    KEK_WINDOW_SET_SIZE_ERROR,
    KEK_WINDOW_GET_SIZE_ERROR,
};

// Keyboard keys (US keyboard layout)
enum keyboard_key {
    // Alphanumeric keys
    KEK_KBD_KEY_APOSTROPHE      = 39,
    KEK_KBD_KEY_COMMA           = 44,
    KEK_KBD_KEY_MINUS           = 45,
    KEK_KBD_KEY_PERIOD          = 46,
    KEK_KBD_KEY_SLASH           = 47,
    KEK_KBD_KEY_ZERO            = 48,
    KEK_KBD_KEY_ONE             = 49,
    KEK_KBD_KEY_TWO             = 50,
    KEK_KBD_KEY_THREE           = 51,
    KEK_KBD_KEY_FOUR            = 52,
    KEK_KBD_KEY_FIVE            = 53,
    KEK_KBD_KEY_SIX             = 54,
    KEK_KBD_KEY_SEVEN           = 55,
    KEK_KBD_KEY_EIGHT           = 56,
    KEK_KBD_KEY_NINE            = 57,
    KEK_KBD_KEY_SEMICOLON       = 59,
    KEK_KBD_KEY_EQUAL           = 61,
    KEK_KBD_KEY_A               = 65,
    KEK_KBD_KEY_B               = 66,
    KEK_KBD_KEY_C               = 67,
    KEK_KBD_KEY_D               = 68,
    KEK_KBD_KEY_E               = 69,
    KEK_KBD_KEY_F               = 70,
    KEK_KBD_KEY_G               = 71,
    KEK_KBD_KEY_H               = 72,
    KEK_KBD_KEY_I               = 73,
    KEK_KBD_KEY_J               = 74,
    KEK_KBD_KEY_K               = 75,
    KEK_KBD_KEY_L               = 76,
    KEK_KBD_KEY_M               = 77,
    KEK_KBD_KEY_N               = 78,
    KEK_KBD_KEY_O               = 79,
    KEK_KBD_KEY_P               = 80,
    KEK_KBD_KEY_Q               = 81,
    KEK_KBD_KEY_R               = 82,
    KEK_KBD_KEY_S               = 83,
    KEK_KBD_KEY_T               = 84,
    KEK_KBD_KEY_U               = 85,
    KEK_KBD_KEY_V               = 86,
    KEK_KBD_KEY_W               = 87,
    KEK_KBD_KEY_X               = 88,
    KEK_KBD_KEY_Y               = 89,
    KEK_KBD_KEY_Z               = 90,

    // Function keys
    KEK_KBD_KEY_SPACE           = 32,
    KEK_KBD_KEY_ESCAPE          = 256,
    KEK_KBD_KEY_ENTER           = 257,
    KEK_KBD_KEY_TAB             = 258,
    KEK_KBD_KEY_BACKSPACE       = 259,
    KEK_KBD_KEY_INSERT          = 260,
    KEK_KBD_KEY_DELETE          = 261,
    KEK_KBD_KEY_RIGHT           = 262,
    KEK_KBD_KEY_LEFT            = 263,
    KEK_KBD_KEY_DOWN            = 264,
    KEK_KBD_KEY_UP              = 265,
    KEK_KBD_KEY_PAGE_UP         = 266,
    KEK_KBD_KEY_PAGE_DOWN       = 267,
    KEK_KBD_KEY_HOME            = 268,
    KEK_KBD_KEY_END             = 269,
    KEK_KBD_KEY_CAPS_LOCK       = 280,
    KEK_KBD_KEY_SCROLL_LOCK     = 281,
    KEK_KBD_KEY_NUM_LOCK        = 282,
    KEK_KBD_KEY_PRINT_SCREEN    = 283,
    KEK_KBD_KEY_PAUSE           = 284,
    KEK_KBD_KEY_F1              = 290,
    KEK_KBD_KEY_F2              = 291,
    KEK_KBD_KEY_F3              = 292,
    KEK_KBD_KEY_F4              = 293,
    KEK_KBD_KEY_F5              = 294,
    KEK_KBD_KEY_F6              = 295,
    KEK_KBD_KEY_F7              = 296,
    KEK_KBD_KEY_F8              = 297,
    KEK_KBD_KEY_F9              = 298,
    KEK_KBD_KEY_F10             = 299,
    KEK_KBD_KEY_F11             = 300,
    KEK_KBD_KEY_F12             = 301,
    KEK_KBD_KEY_LEFT_SHIFT      = 340,
    KEK_KBD_KEY_LEFT_CONTROL    = 341,
    KEK_KBD_KEY_LEFT_ALT        = 342,
    KEK_KBD_KEY_LEFT_SUPER      = 343,
    KEK_KBD_KEY_RIGHT_SHIFT     = 344,
    KEK_KBD_KEY_RIGHT_CONTROL   = 345,
    KEK_KBD_KEY_RIGHT_ALT       = 346,
    KEK_KBD_KEY_RIGHT_SUPER     = 347,
    KEK_KBD_KEY_KB_MENU         = 348,
    KEK_KBD_KEY_LEFT_BRACKET    = 91,
    KEK_KBD_KEY_BACKSLASH       = 92,
    KEK_KBD_KEY_RIGHT_BRACKET   = 93,
    KEK_KBD_KEY_GRAVE           = 96,

    // Keypad keys
    KEK_KBD_KEY_KP_0            = 320,
    KEK_KBD_KEY_KP_1            = 321,
    KEK_KBD_KEY_KP_2            = 322,
    KEK_KBD_KEY_KP_3            = 323,
    KEK_KBD_KEY_KP_4            = 324,
    KEK_KBD_KEY_KP_5            = 325,
    KEK_KBD_KEY_KP_6            = 326,
    KEK_KBD_KEY_KP_7            = 327,
    KEK_KBD_KEY_KP_8            = 328,
    KEK_KBD_KEY_KP_9            = 329,
    KEK_KBD_KEY_KP_DECIMAL      = 330,
    KEK_KBD_KEY_KP_DIVIDE       = 331,
    KEK_KBD_KEY_KP_MULTIPLY     = 332,
    KEK_KBD_KEY_KP_SUBTRACT     = 333,
    KEK_KBD_KEY_KP_ADD          = 334,
    KEK_KBD_KEY_KP_ENTER        = 335,
    KEK_KBD_KEY_KP_EQUAL        = 336
};

enum mouse_button {
    KEK_MOUSE_BUTTON_LEFT   = 0,
    KEK_MOUSE_BUTTON_RIGHT  = 1,
    KEK_MOUSE_BUTTON_MIDDLE = 2
};


enum config_type {
    KEK_CFG_MEM_STACK_CAPACITY,
    KEK_CFG_MEM_TAG_CAPACITY,
    KEK_CFG_MEM_CAMERA_CAPACITY,
    KEK_CFG_MEM_VERTEX_BUFFER_CAPACITY,
    KEK_CFG_MEM_RENDER_CAPACITY,
    KEK_CFG_MEM_SHADER_CAPACITY,
    KEK_CFG_MEM_TEXTURE_CAPACITY,
    KEK_CFG_MEM_SCENE_CAPACITY,
    KEK_CFG_MEM_SCENE_ENTITY_CAPACITY,
    KEK_CFG_MEM_ANIMATION_CAPACITY,
    KEK_CFG_MEM_ENTITY_CAPACITY,
    KEK_CFG_MEM_ENTITY_USER_DATA_SIZE,
    KEK_CFG_MEM_ENTITY_TYPE_CAPACITY,
    KEK_CFG_MEM_KEY_BIND_ALIAS_CAPACITY,
    KEK_CFG_NUM_TYPES,
};

union vec2 {
	struct {
		float v[2];
	};

	struct {
		float x;
		float y;
	};
};

union vec3 {
	struct {
		float v[3];
	};

	struct {
		float x;
		float y;
		float z;
	};

	struct {
		float r;
		float g;
		float b;
	};

	struct {
		union vec2 xy;
	};

	struct {
		union vec2 rg;
	};

	struct {
		float unused0;
		union vec2 yz;
	};

	struct {
		float unused1;
		union vec2 gb;
	};
};

union vec4 {
	struct {
		float v[4];
	};

	struct {
		float x;
		float y;
		float z;
		float w;
	};

	struct {
		float r;
		float g;
		float b;
		float a;
	};

	struct {
		union vec3 xyz;
	};

	struct {
		union vec3 rgb;
	};
	struct {
		float unused0;
		union vec3 yzw;
	};
	struct {
		float unused1;
		union vec3 gba;
	};

	struct {
		union vec2 xy;
	};

	struct {
		union vec2 rg;
	};

	struct {
		float unused2;
		union vec2 yz;
	};
	struct {
		float unused3;
		union vec2 gb;
	};
	struct {
		float unused4;
		float unused5;
		union vec2 zw;
	};
	struct {
		float unused6;
		float unused7;
		union vec2 ba;
	};
};

struct rect2 {
	union vec2 position;
	union vec2 size;
};

struct cube {
	union vec3 position;
	union vec3 size;
};

struct mat4 {
	float m[4][4]; //[row][column]
};

struct camera {
    union vec3 position;
    float zoom;
};

struct texture {
    GLuint id;
    unsigned int height;
    unsigned int width;
    bool loaded;
};

struct animation_frame {
    struct texture *texture;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    float duration; // in seconds
};

#define MAX_ANIMATION_FRAMES 16
struct animation {
    struct animation_frame frames[MAX_ANIMATION_FRAMES];
    size_t frame_count;
    bool loop;
};

struct vertex_buffer {
    GLuint vao;
    GLuint vbo;
    void *map_buffer;
    size_t capacity;
    size_t size;
};

struct render {
    struct shader *shader;
    struct vertex_buffer *vb;
    KEKRenderFn draw_callback;
    void *ctx;
};

struct shader {
    GLuint shader;
};

struct entity {
    bool destroy;
    uint32_t type;
    union vec3 position;
    union vec3 velocity;
    union vec3 rotation;
    union vec3 size;

    struct texture *texture;
    struct animation *animation;
    unsigned int animation_frame;
    float animation_frame_time;
    float animation_speed;

    struct entity *scene_next_entity;
};

struct scene {
    struct entity *entities;
    struct render *render_default;
    struct camera *camera;
    size_t entity_count;
};

struct mem_pool {
    uint8_t *buffer;
    size_t capacity;
    size_t stride;
    size_t free_head;
    size_t free_count;
    size_t use_count;
};

struct physics_body {
    int dummy;
};

struct physics_constraint {
    int dummy;
};

