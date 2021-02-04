#include "pal.h"
#include "GLFW/glfw3.h"


float pal_time_live(void)
{
    return (float)glfwGetTime();
}
