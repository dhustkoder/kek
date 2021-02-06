#include "pal.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <assert.h>
#include <stdio.h>

static PALReturn pal_glfw_has_error(PALWindow *window)
{
    return (glfwGetError(NULL) == GLFW_NO_ERROR) ? PAL_OK : PAL_ERROR; 
}

PALReturn pal_window_init(void)
{
    PALWindow window;

    window.glfw = NULL;

    if(glfwInit() != GLFW_TRUE)
        return PAL_ERROR;

    return PAL_OK;
}

PALReturn pal_window_open(PALWindow *window, const char *title, unsigned int width, unsigned int height)
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

bool pal_window_is_open(PALWindow *window)
{
    return (window->glfw) ? true : false;
}

PALReturn pal_window_terminate(PALWindow *window)
{
    window->glfw = NULL;

    glfwTerminate();

    return pal_glfw_has_error(window);
}

PALReturn pal_window_set_title(PALWindow *window, const char *title)
{
    glfwSetWindowTitle(window->glfw, title);

    return pal_glfw_has_error(window);
}

PALReturn pal_window_set_size(PALWindow *window, unsigned int width, unsigned int height)
{
    int fbwidth;
    int fbheight; 

    glfwSetWindowSize(window->glfw, (int)width, (int)height);

    return pal_glfw_has_error(window);
}

PALReturn pal_window_get_size(PALWindow *window, unsigned int *width, unsigned int *height)
{
    glfwGetWindowSize(window->glfw, width, height);

    return pal_glfw_has_error(window);
}

PALReturn pal_window_get_framebuffer_size(PALWindow *window, unsigned int *width, unsigned int *height)
{
    glfwGetFramebufferSize(window->glfw, width, height);

    return pal_glfw_has_error(window);
}

PALReturn pal_window_poll(PALWindow *window)
{
    glfwPollEvents();

    return pal_glfw_has_error(window);
}

PALReturn pal_window_swap_buffers(PALWindow *window)
{
    glfwSwapBuffers(window->glfw);

    return pal_glfw_has_error(window);
}

PALReturn pal_window_is_keyboard_pressed(PALWindow *window, int key, bool *pressed)
{
    *pressed = (glfwGetKey(window->glfw, key) != GLFW_RELEASE) ? true : false;

    return pal_glfw_has_error(window);
}


PALReturn pal_window_is_mouse_button_pressed(PALWindow *window, int button, bool *pressed)
{
    *pressed = (glfwGetMouseButton(window->glfw, button) != GLFW_RELEASE) ? true : false;

    return pal_glfw_has_error(window);
}

PALReturn pal_window_get_mouse_position(PALWindow *window, float *x, float *y)
{
    double xpos, ypos;
    glfwGetCursorPos(window->glfw, &xpos, &ypos);

    *x = (float)xpos;
    *y = (float)ypos;

    return pal_glfw_has_error(window);
}
