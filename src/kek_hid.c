#include "kek.h"
#include <assert.h>

typedef struct
{
    int alias;
    enum keyboard_key key;
} KeyboardBind;

static KeyboardBind *bindings = NULL;
static size_t binding_count = 0;
static size_t binding_capacity = 0;

static bool mousebutton_last[KEK_NUM_MOUSE_BUTTONS];

void init_hid(size_t alias_capacity)
{
    bindings = memstack_push(alias_capacity * sizeof(KeyboardBind));
    binding_capacity = alias_capacity;

    for(size_t i = 0; i < KEK_NUM_MOUSE_BUTTONS; ++i)
        mousebutton_last[i] = false;
}

void update_hid(void)
{
    for(size_t i = 0; i < KEK_NUM_MOUSE_BUTTONS; ++i)
        mousebutton_last[i] = is_mouse_button_pressed(i);
}

void bind_hid_alias_to_key(int alias, enum keyboard_key key)
{
    assert(binding_count < binding_capacity); 

    bindings[binding_count].alias = alias;
    bindings[binding_count].alias = key;
    ++binding_count;
}

void unbind_hid_key(enum keyboard_key key)
{
    size_t i = 0;
    while(i < binding_count)
    {
        if(bindings[i].key == key)
        {
            bindings[i] = bindings[binding_count - 1];
            --binding_count;
        }
        else 
        {
            ++i;
        }
    }
}

void unbind_hid_alias(int alias)
{
    size_t i = 0;
    while(i < binding_count)
    {
        if(bindings[i].alias == alias)
        {
            bindings[i] = bindings[binding_count - 1];
            --binding_count;
        }
        else 
        {
            ++i;
        }
    }
}

bool is_hid_alias_pressed(int alias)
{
    for(size_t i = 0; i < binding_count; ++i)
    {
        if(bindings[i].alias == alias)
        {
            if(is_hid_key_pressed(bindings[i].key))
                return true;
        }
    }

    return false;
}

bool is_hid_key_pressed(enum keyboard_key key)
{
    PALWindow *window = get_pal_window();

    bool pressed = false;

    pal_is_keyboard_pressed(window, key, &pressed);

    return pressed;
}

bool is_mouse_button_pressed_frame(enum mouse_button button)
{
    bool pressed = is_mouse_button_pressed(button);

    return (pressed && !mousebutton_last[button]);
}

bool is_mouse_button_released_frame(enum mouse_button button)
{
    bool pressed = is_mouse_button_pressed(button);

    return (!pressed && mousebutton_last[button]);
}

bool is_mouse_button_pressed(enum mouse_button button)
{
    PALWindow *window = get_pal_window();

    bool pressed = false;

    pal_is_mouse_button_pressed(window, button, &pressed);

    return pressed;
}

bool is_mouse_button_released(enum mouse_button button)
{
    return !is_mouse_button_pressed(button);
}

Vec2 get_mouse_position(void)
{
    PALWindow *window = get_pal_window();

    Vec2 p;
    unsigned int window_width;
    unsigned int window_height;

    get_window_size(&window_width, &window_height);

    pal_get_mouse_position(window, &p.x, &p.y);

    p.y = (float)window_height - p.y;

    
    return p;
}
