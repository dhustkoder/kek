#pragma once

#if defined(PAL_USE_GLFW)
#include "pal_window_glfw.h"

#else
#error "UNDEFINED PRIVATE WINDOW API"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef enum pal_return {
    PAL_OK,
    PAL_ERROR,
} PALReturn;

// user implemented
extern void pal_main(int argc, char **argv);

// Window Management
PALReturn pal_window_init(void);
PALReturn pal_window_open(PALWindow *window, const char *title, unsigned int width, unsigned int height);
bool pal_window_is_open(PALWindow *window);
PALReturn pal_window_terminate(PALWindow *window);
PALReturn pal_window_set_title(PALWindow *window, const char *title);
PALReturn pal_window_get_size(PALWindow *window, unsigned int *width, unsigned int *height);
PALReturn pal_window_set_size(PALWindow *window, unsigned int width, unsigned int height);
PALReturn pal_window_get_framebuffer_size(PALWindow *window, unsigned int *width, unsigned int *height);
PALReturn pal_window_poll(PALWindow *window);
PALReturn pal_window_swap_buffers(PALWindow *window);

PALReturn pal_window_is_keyboard_pressed(PALWindow *window, int key, bool *pressed);
PALReturn pal_window_is_mouse_button_pressed(PALWindow *window, int button, bool *pressed);
PALReturn pal_window_get_mouse_position(PALWindow *window, float *x, float *y);

float pal_time_live(void);

// File management
PALReturn pal_file_to_buffer(const char *file, uint8_t *buffer, size_t *size_bytes, int capacity);
PALReturn pal_file_get_size(const char *file, size_t *size);

