#pragma once

#if defined(PAL_USE_GLFW)
#include "pal_window_glfw.h"

#else
#error "UNDEFINED PRIVATE WINDOW API"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


enum pal_return {
    PAL_OK,
    PAL_ERROR,
};

// user implemented
extern void pal_main(int argc, char **argv);

// Window Management
enum pal_return pal_window_init(void);
enum pal_return pal_window_open(struct pal_window *window, const char *title, unsigned int width, unsigned int height);
bool pal_window_is_open(struct pal_window *window);
enum pal_return pal_window_terminate(struct pal_window *window);
enum pal_return pal_window_set_title(struct pal_window *window, const char *title);
enum pal_return pal_window_get_size(struct pal_window *window, unsigned int *width, unsigned int *height);
enum pal_return pal_window_set_size(struct pal_window *window, unsigned int width, unsigned int height);
enum pal_return pal_window_get_framebuffer_size(struct pal_window *window, unsigned int *width, unsigned int *height);
enum pal_return pal_window_poll(struct pal_window *window);
enum pal_return pal_window_swap_buffers(struct pal_window *window);

enum pal_return pal_window_is_keyboard_pressed(struct pal_window *window, int key, bool *pressed);
enum pal_return pal_window_is_mouse_button_pressed(struct pal_window *window, int button, bool *pressed);
enum pal_return pal_window_get_mouse_position(struct pal_window *window, float *x, float *y);

float pal_time_live(void);

// File management
enum pal_return pal_file_to_buffer(const char *file, uint8_t *buffer, size_t *size_bytes, int capacity);
enum pal_return pal_file_get_size(const char *file, size_t *size);

