#include "pal.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <assert.h>
#include <stdio.h>

static enum pal_return pal_glfw_has_error(struct pal_window *window)
{
    return (glfwGetError(NULL) == GLFW_NO_ERROR) ? PAL_OK : PAL_ERROR; 
}

enum pal_return pal_window_init(void)
{
    struct pal_window window;

    window.glfw = NULL;

    if(glfwInit() != GLFW_TRUE)
        return PAL_ERROR;

    return PAL_OK;
}

enum pal_return pal_window_open(struct pal_window *window, const char *title, unsigned int width, unsigned int height)
{
    GLFWwindow *gw;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);

    window->glfw = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window->glfw)
    {
        glfwTerminate();
        return PAL_ERROR;
    }
    
    glfwMakeContextCurrent(window->glfw);

    glfwSetWindowUserPointer(window->glfw, window); 

    gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 


    return PAL_OK;
}

bool pal_window_is_open(struct pal_window *window)
{
    return (window->glfw) ? true : false;
}

enum pal_return pal_window_terminate(struct pal_window *window)
{
    window->glfw = NULL;

    glfwTerminate();

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_set_title(struct pal_window *window, const char *title)
{
    glfwSetWindowTitle(window->glfw, title);

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_set_size(struct pal_window *window, unsigned int width, unsigned int height)
{
    int fbwidth;
    int fbheight; 

    glfwSetWindowSize(window->glfw, (int)width, (int)height);

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_get_size(struct pal_window *window, unsigned int *width, unsigned int *height)
{
    glfwGetWindowSize(window->glfw, width, height);

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_get_framebuffer_size(struct pal_window *window, unsigned int *width, unsigned int *height)
{
    glfwGetFramebufferSize(window->glfw, width, height);

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_poll(struct pal_window *window)
{
    glfwPollEvents();

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_swap_buffers(struct pal_window *window)
{
    glfwSwapBuffers(window->glfw);

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_is_keyboard_pressed(struct pal_window *window, int key, bool *pressed)
{
    *pressed = (glfwGetKey(window->glfw, key) != GLFW_RELEASE) ? true : false;

    return pal_glfw_has_error(window);
}


enum pal_return pal_window_is_mouse_button_pressed(struct pal_window *window, int button, bool *pressed)
{
    *pressed = (glfwGetMouseButton(window->glfw, button) != GLFW_RELEASE) ? true : false;

    return pal_glfw_has_error(window);
}

enum pal_return pal_window_get_mouse_position(struct pal_window *window, float *x, float *y)
{
    double xpos, ypos;
    glfwGetCursorPos(window->glfw, &xpos, &ypos);

    *x = (float)xpos;
    *y = (float)ypos;

    return pal_glfw_has_error(window);
}
