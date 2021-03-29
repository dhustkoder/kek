#include "kek.h"
#include "pal.h"
#include <stdbool.h>
#include <stdlib.h>

static MemPool pool;

static int compile_shader(GLuint shader, const char *code, char *error_buffer, size_t error_buffer_capacity);

void init_shader(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Shader));
}

Shader *get_shader(int shaderid)
{
    return mempool_get_addr(&pool, shaderid);
}

int create_shader(void)
{
     Shader *inst = mempool_take(&pool);

    if(inst == NULL)
        return -1;

    inst->id = mempool_get_slot(&pool, inst);
    inst->shader = gl_create_program();

    return inst->id;
}

void destroy_shader(int shaderid)
{
    Shader *shader = get_shader(shaderid);

    gl_delete_program(shader->shader);
    shader->shader = 0;

    mempool_release(&pool, shader);
}

int load_shader_files(int shaderid, const char *vert_file, const char *frag_file)
{
    size_t vert_size = 0;
    size_t frag_size = 0;
    char *vert_buffer;
    char *frag_buffer;

    int result;
    
    pal_file_get_size(vert_file, &vert_size);
    pal_file_get_size(frag_file, &frag_size);
    
    vert_buffer  = memstack_push(vert_size + 1); 
    frag_buffer  = memstack_push(frag_size + 1); 

    pal_file_to_buffer(vert_file, (uint8_t *)vert_buffer, &vert_size, vert_size);
    pal_file_to_buffer(frag_file, (uint8_t *)frag_buffer, &frag_size, frag_size);
    vert_buffer[vert_size] = '\0';
    frag_buffer[frag_size] = '\0';

    result = load_shader_buffer(shaderid, vert_buffer, frag_buffer);

    memstack_pop(frag_buffer);
    memstack_pop(vert_buffer);

    return result;
}

int load_shader_buffer(int shaderid, const char *vert_buffer, const char *frag_buffer)
{
    Shader *shader = get_shader(shaderid);
    static const size_t ERROR_BUFFER_CAPACITY = 2048;
    char *error_buffer;

    GLuint frag_shader;
    GLuint vert_shader;

    PALReturn vert_result;
    PALReturn frag_result;

    error_buffer = memstack_push(ERROR_BUFFER_CAPACITY); 

    vert_shader = gl_create_shader(GL_VERTEX_SHADER);
    frag_shader = gl_create_shader(GL_FRAGMENT_SHADER);

    vert_result = compile_shader(vert_shader, 
                        vert_buffer, error_buffer, ERROR_BUFFER_CAPACITY);
    if(!vert_result)
        loge("Vertex shader ompile failed: %s\n\n buffer:\n%s\n\n", error_buffer, vert_buffer);

    frag_result = compile_shader(frag_shader, 
                        frag_buffer, error_buffer, ERROR_BUFFER_CAPACITY);

    if(!vert_result)
        loge("Fragment shader compile failed: %s\n\n buffer:\n%s\n\n", error_buffer, frag_buffer);

    gl_attach_shader(shader->shader, vert_shader);
    gl_attach_shader(shader->shader, frag_shader);
    gl_link_program(shader->shader);

    gl_delete_shader(vert_shader);
    gl_delete_shader(frag_shader);

    memstack_pop(error_buffer);

    if(!vert_result)
        return KEK_ERROR;

    if(!frag_result)
        return KEK_ERROR;

    return KEK_OK;
}

int bind_shader(int shaderid)
{
    Shader *shader = get_shader(shaderid);
    gl_use_program(shader->shader);

    return KEK_OK;
}

static int compile_shader(GLuint shader, const char *code, char *error_buffer, size_t error_buffer_capacity)
{
    gl_shader_source(shader, 1, (const char **)&code, 0);
    gl_compile_shader(shader);

    GLint result = GL_FALSE;

    gl_get_shaderiv(shader, GL_COMPILE_STATUS, &result);

    if(result != GL_TRUE)
    {
        GLint log_size = 0;

		if (error_buffer && error_buffer_capacity > 0) 
        {
			gl_get_shaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
			gl_get_shader_info_log(shader, (GLsizei)error_buffer_capacity, 0, error_buffer);

            error_buffer[error_buffer_capacity - 1] = '\0';

			return KEK_ERROR;
		}
    }

    return KEK_OK;
}

