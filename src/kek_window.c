#include "kek.h"
#include "kek_private.h"
#include "pal.h"
#include <string.h>

#define MAX_TITLE_LEN 1024

static PALWindow window;
static Vec4 screen_color = { 0, 0, 0, 0};

int window_init(void)
{
    PALReturn ret;

    ret = pal_window_init();

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_set_title(const char *title)
{
    PALReturn ret;

    ret = pal_window_set_title(&window, title);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_set_size(unsigned int width, unsigned int height)
{
    PALReturn ret;

    ret = pal_window_set_size(&window, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_get_size(unsigned int *width, unsigned int *height)
{
    PALReturn ret;

    ret = pal_window_get_size(&window, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_poll(void)
{
    PALReturn ret;

    ret = pal_window_poll(&window);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_swap_buffers(void)
{
    PALReturn ret;

    ret = pal_window_swap_buffers(&window);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_open(const char *title, unsigned int width, unsigned int height)
{
    PALReturn ret;
    
    ret = pal_window_open(&window, title, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

PALWindow *window_get_pal(void)
{
    return &window;
}

void window_clearscreen(void)
{
    gl_clear_color(screen_color.r, screen_color.g, screen_color.b, screen_color.a);
    gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_set_clearscreen_color(Vec4 color)
{
    screen_color =  color;
}

