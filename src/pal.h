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
PALReturn pal_init_window(void);
PALReturn pal_open_window(PALWindow *window, const char *title, unsigned int width, unsigned int height);
bool pal_is_window_open(PALWindow *window);
PALReturn pal_terminate_window(PALWindow *window);
PALReturn pal_window_title(PALWindow *window, const char *title);
PALReturn pal_get_window_size(PALWindow *window, unsigned int *width, unsigned int *height);
PALReturn pal_window_size(PALWindow *window, unsigned int width, unsigned int height);
PALReturn pal_get_window_framebuffer_size(PALWindow *window, unsigned int *width, unsigned int *height);
PALReturn pal_poll_window(PALWindow *window);
PALReturn pal_swap_window_buffers(PALWindow *window);

PALReturn pal_is_keyboard_pressed(PALWindow *window, int key, bool *pressed);
PALReturn pal_is_mouse_button_pressed(PALWindow *window, int button, bool *pressed);
PALReturn pal_get_mouse_position(PALWindow *window, float *x, float *y);

float pal_time(void);

// File management
PALReturn pal_file_to_buffer(const char *file, uint8_t *buffer, size_t *size_bytes, size_t capacity);
PALReturn pal_file_get_size(const char *file, size_t *size);

