#include "kek_gl.h"
#include "glad/gl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void check_gl_error(const char *stmt, char *fname, int line)
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        //todo: instead of print and abort, execute a registered error handler
        printf("OpenGL error 0x%x at %s:%i - %s\n", err, fname, line,stmt);
        abort();
    }
}

