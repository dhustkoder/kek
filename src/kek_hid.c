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

void hid_init(size_t alias_capacity)
{
    bindings = mem_stack_push(alias_capacity * sizeof(KeyboardBind));
    binding_capacity = alias_capacity;
}

void hid_bind_alias_to_key(int alias, enum keyboard_key key)
{
    assert(binding_count < binding_capacity); 

    bindings[binding_count].alias = alias;
    bindings[binding_count].alias = key;
    ++binding_count;
}

void hid_unbind_key(enum keyboard_key key)
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

void unbind_alias(int alias)
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

bool hid_is_alias_pressed(int alias)
{
    for(size_t i = 0; i < binding_count; ++i)
    {
        if(bindings[i].alias == alias)
        {
            if(hid_is_key_pressed(bindings[i].key))
                return true;
        }
    }

    return false;
}

bool hid_is_key_pressed(enum keyboard_key key)
{
    PALWindow *window = window_get_pal();

    bool pressed = false;

    pal_window_is_keyboard_pressed(window, key, &pressed);

    return pressed;
}

bool hid_is_mouse_button_pressed(enum mouse_button button)
{
    PALWindow *window = window_get_pal();

    bool pressed = false;

    pal_window_is_mouse_button_pressed(window, button, &pressed);

    return pressed;
}

Vec2 hid_get_mouse_position(void)
{
    PALWindow *window = window_get_pal();

    Vec2 p;
    unsigned int window_width;
    unsigned int window_height;

    window_get_size(&window_width, &window_height);

    pal_window_get_mouse_position(window, &p.x, &p.y);

    p.y = (float)window_height - p.y;

    
    return p;
}
