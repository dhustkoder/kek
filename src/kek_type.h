#pragma once

#include "kek_gl.h"
#include "pal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define KEK_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define KEK_STRUCT_ATTRIB_SIZE(type, member) sizeof(((type *)0)->member)

#define KEK_PI M_PI
#define KEK_PI_2 (PJ_PI * 0.5f)
#define KEK_MIN(a, b) (((a) < (b)) ? (a) : (b)) 
#define KEK_MAX(a, b) (((a) > (b)) ? (a) : (b)) 

// forward declarations
typedef struct entity Entity;
typedef struct camera Camera;
typedef struct render Render;
typedef struct shader Shader;
typedef struct event Event;
typedef struct spatial_node SpatialNode;
typedef union vec3 Vec3;


typedef void (*EventCallback)(Event *event, void *ctx);
typedef void (*EntityTerminateFn)(int entityid, void *ctx); 
typedef void (*EntityUpdateFn)(int entityid, void *ctx); 
typedef void (*EntityQueryFn)(int id, void *ctx);
typedef void (*CollisionFn)(int entityid_a, int entityid_b, void *ctx);
typedef void (*RenderEntitiesFn)(Render *render, Camera *camera, int *entities, size_t count, void *ctx);
typedef void (*RenderLinesFn)(Render *render, Camera *camera, Vec3 *points, size_t count, void *ctx);
typedef void (*SceneQueryEntityFn)(int entityid, void *ctx);
typedef void (*SpatialMapQueryFn)(SpatialNode *node, void *ctx);
typedef void (*CameraUpdateFn)(int cameraid, void *ctx);


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
typedef enum keyboard_key {
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
    KidEK_KBD_KEY_PAGE_UP         = 266,
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
} KeyboardKey;

typedef enum mouse_button {
    KEK_MOUSE_BUTTON_LEFT   = 0,
    KEK_MOUSE_BUTTON_RIGHT  = 1,
    KEK_MOUSE_BUTTON_MIDDLE = 2
} MouseButton;


typedef enum config_type {
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
    KEK_CFG_MEM_SPATIAL_MAP_CAPACITY,
    KEK_CFG_MEM_KEY_BIND_ALIAS_CAPACITY,
    KEK_CFG_SOUND_SOURCE_CAPACITY,
    KEK_CFG_SOUND_BUFFER_CAPACITY,
    KEK_CFG_SOUND_CAPACITY,
    KEK_CFG_NUM_TYPES,
} ConfigType;

typedef union vec2 {
	struct {
		float v[2];
	};

	struct {
		float x;
		float y;
	};
} Vec2;

typedef union vec3 {
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
		Vec2 xy;
	};

	struct {
		Vec2 rg;
	};

	struct {
		float unused0;
		Vec2 yz;
	};

	struct {
		float unused1;
		Vec2 gb;
	};
} Vec3;

typedef union vec4 {
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
		Vec3 xyz;
	};

	struct {
		Vec3 rgb;
	};
	struct {
		float unused0;
		Vec3 yzw;
	};
	struct {
		float unused1;
		Vec3 gba;
	};

	struct {
		Vec2 xy;
	};

	struct {
		Vec2 rg;
	};

	struct {
		float unused2;
		Vec2 yz;
	};
	struct {
		float unused3;
		Vec2 gb;
	};
	struct {
		float unused4;
		float unused5;
		Vec2 zw;
	};
	struct {
		float unused6;
		float unused7;
		Vec2 ba;
	};
} Vec4;

typedef struct rect2 {
	Vec2 position;
	Vec2 size;
} Rect;

typedef struct cube {
	Vec3 position;
	Vec3 size;
} Cube;

typedef struct mat4{
	float m[4][4]; //[row][column]
} Mat4;

typedef struct mem_pool {
    uint8_t *buffer;
    size_t capacity;
    size_t stride;
    size_t free_head;
    size_t free_count;
    size_t use_count;
} MemPool;

typedef struct camera {
    int id;
    Vec3 position;
    Vec3 offset;
    float zoom;
    CameraUpdateFn update_cb;
    void *update_ctx;
} Camera;

typedef struct texture {
    int id;
    GLuint glid;
    unsigned int height;
    unsigned int width;
    bool loaded;
} Texture;

typedef struct vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec4 colormask;
} Vertex;

typedef struct circle_vertex {
    Vec3 position;
    Vec3 center;
    float radius;
    Vec4 colormask;
} CircleVertex;
#if 0
typedef enum material_property_type{
    MATERIAL_PROPERTY_VEC2,
    MATERIAL_PROPERTY_VEC3,
    MATERIAL_PROPERTY_VEC4,
    MATERIAL_PROPERTY_MAT4,
    MATERIAL_PROPERTY_TEXTURE,
    MATERIAL_PROPERTY_INT,
    MATERIAL_PROPERTY_UINT,
    MATERIAL_PROPERTY_FLOAT,
} MaterialPropertyType;

typedef struct material_property
{
    MaterialPropertyType type;
    const char *name;
    union {
        Vec2 vec2;
        Vec3 vec3;
        Vec4 vec4;
        Mat4 mat4;
        Texture *texture;
        int i;
        unsigned int ui;
        float f;
    };
} MaterialProperty;

#define MATERIAL_PROPERTY_CAPACITY 16
typedef struct material {
    MaterialProperty properties[MATERIAL_PROPERTY_CAPACITY];
    size_t count;
} Material;
#endif

typedef struct tilemap_index
{
    int spritesheet;
    Vec2 uv0;
    Vec2 uv1;
} TilemapSpritesheetClip;

#define TILEMAP_SPRITESHEET_CAPACITY 8
#define TILEMAP_INDEX_CAPACITY 1024
typedef struct tilemap {
    int id;
    int textures[TILEMAP_SPRITESHEET_CAPACITY];
    size_t texture_count;

    TilemapSpritesheetClip indexmap[TILEMAP_INDEX_CAPACITY];

    int *map;
    int num_cells_x;
    int num_cells_y;
    Vec2 cell_size;
    int cell_pixel_width;
    int cell_pixel_height;
} Tilemap;

typedef struct animation_frame {
    int texture;
    int pixel_width;
    int pixel_height;
    Vec2 uv0;
    Vec2 uv1;
    float duration; // in seconds
} AnimationFrame;

#define MAX_ANIMATION_FRAMES 16
typedef struct animation {
    int id;
    AnimationFrame frames[MAX_ANIMATION_FRAMES];
    size_t frame_count;
    bool loop;
} Animation;

typedef struct vertexbuffer {
    int id;
    GLuint vao;
    GLuint vbo;
    void *map_buffer;
    size_t capacity;
    size_t size;
} VertexBuffer;


typedef struct render {
    int id;
    int shader;
    int vb; // vertex buffer
    void *ctx;
} Render;


typedef struct shader {
    int id;
    GLuint shader;
} Shader;

typedef struct spatialmap SpatialMap;

#define SPATIAL_NODE_XSPAN 16
#define SPATIAL_NODE_YSPAN 16

typedef struct spatial_node {
    int x;
    int y;
    void *data;
    SpatialNode *next;
} SpatialNode;

typedef struct spatialmap {
    SpatialNode *nodes[SPATIAL_NODE_YSPAN][SPATIAL_NODE_XSPAN];
    int basex;
    int basey;
} SpatialMap;


enum {
    COLLIDER_NONE,
    COLLIDER_CIRCLE,
    COLLIDER_RECT,
};
typedef struct collider {
    int type;
    bool dynamic;
    uint32_t mask;      // i collide with
    uint32_t category; // i am a
    CollisionFn collision_fn;
    void *ctx;
    union {
        float radius;
        Vec2 rect;
    };
} Collider;

typedef struct entity {
    int id;
    bool destroy;
    uint32_t type;
    Vec3 position;
    Vec3 velocity;
    float gravity_scale;
    Vec3 last_position;
    Vec3 last_velocity;
    Vec3 rotation;
    Vec3 texture_rotation;
    Vec3 scale;
    Vec4 colormask;
    Collider collider;

    int texture;
    Animation *animation;
    unsigned int animation_frame;
    float animation_frame_time;
    float animation_speed;

    SpatialNode snode;
    Entity *scene_next_entity;
} Entity;

typedef struct scene {
    int id;
    Entity *entities;
    int render_entity;
#if 0
    Render *render_entity_box;
    Render *render_rect;
    Render *render_circle;
    Render *render_spatialmap;
#endif
    int camera;
    size_t entity_count;
} Scene;

typedef struct physics_body {
    int dummy;
} PhysicsBody;

typedef struct physics_constraint {
    int dummy;
} PhysicsConstraint;

typedef struct event {
    int id;
    void *data;
} Event;

