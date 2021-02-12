#include "kek.h"
#include "kek_private.h"
#include "pal.h"
#include <string.h>

#define MAX_TITLE_LEN 1024

static PALWindow window;
static Vec4 screen_color = { 0, 0, 0, 0};

int init_window(void)
{
    PALReturn ret;

    ret = pal_init_window();

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_title(const char *title)
{
    PALReturn ret;

    ret = pal_window_title(&window, title);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int window_size(unsigned int width, unsigned int height)
{
    PALReturn ret;

    ret = pal_window_size(&window, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int get_window_size(unsigned int *width, unsigned int *height)
{
    PALReturn ret;

    ret = pal_get_window_size(&window, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int poll_window(void)
{
    PALReturn ret;

    ret = pal_poll_window(&window);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int swap_window_buffers(void)
{
    PALReturn ret;

    ret = pal_swap_window_buffers(&window);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

int open_window(const char *title, unsigned int width, unsigned int height)
{
    PALReturn ret;
    
    ret = pal_open_window(&window, title, width, height);

    return (ret == PAL_OK) ? KEK_OK : KEK_ERROR;
}

PALWindow *get_pal_window(void)
{
    return &window;
}

void clearscreen(void)
{
    gl_clear_color(screen_color.r, screen_color.g, screen_color.b, screen_color.a);
    gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clearscreen_color(Vec4 color)
{
    screen_color =  color;
}

