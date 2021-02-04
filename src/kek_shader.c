#include "kek.h"
#include "pal.h"
#include <stdbool.h>
#include <stdlib.h>

static struct mem_pool pool;

static int shader_compile(GLuint shader, const char *code, char *error_buffer, size_t error_buffer_capacity);

void shader_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct shader));
}

struct shader *shader_create(void)
{
     struct shader *inst = mem_pool_take(&pool);

    if(inst == NULL)
        return NULL;

    inst->shader = gl_create_program();

    return inst;
}

void shader_destroy(struct shader *shader)
{
    gl_delete_program(shader->shader);
    shader->shader = 0;

    mem_pool_release(&pool, shader);
}

int shader_load_files(struct shader *shader, const char *vert_file, const char *frag_file)
{
    size_t vert_size = 0;
    size_t frag_size = 0;
    char *vert_buffer;
    char *frag_buffer;

    int result;
    
    pal_file_get_size(vert_file, &vert_size);
    pal_file_get_size(frag_file, &frag_size);
    
    vert_buffer  = mem_stack_push(vert_size + 1); 
    frag_buffer  = mem_stack_push(frag_size + 1); 

    pal_file_to_buffer(vert_file, (uint8_t *)vert_buffer, &vert_size, vert_size);
    pal_file_to_buffer(frag_file, (uint8_t *)frag_buffer, &frag_size, frag_size);
    vert_buffer[vert_size] = '\0';
    frag_buffer[frag_size] = '\0';

    result = shader_load_buffer(shader, vert_buffer, frag_buffer);

    mem_stack_pop(frag_buffer);
    mem_stack_pop(vert_buffer);

    return result;
}

int shader_load_buffer(struct shader *shader, const char *vert_buffer, const char *frag_buffer)
{
    static const size_t ERROR_BUFFER_CAPACITY = 2048;
    char *error_buffer;

    GLuint frag_shader;
    GLuint vert_shader;

    enum pal_return vert_result;
    enum pal_return frag_result;

    error_buffer = mem_stack_push(ERROR_BUFFER_CAPACITY); 

    vert_shader = gl_create_shader(GL_VERTEX_SHADER);
    frag_shader = gl_create_shader(GL_FRAGMENT_SHADER);

    vert_result = shader_compile(vert_shader, 
                        vert_buffer, error_buffer, ERROR_BUFFER_CAPACITY);

    frag_result = shader_compile(frag_shader, 
                        frag_buffer, error_buffer, ERROR_BUFFER_CAPACITY);

    gl_attach_shader(shader->shader, vert_shader);
    gl_attach_shader(shader->shader, frag_shader);
    gl_link_program(shader->shader);

    gl_delete_shader(vert_shader);
    gl_delete_shader(frag_shader);

    mem_stack_pop(error_buffer);

    return KEK_OK;
}

int shader_bind(struct shader *shader)
{
    gl_use_program(shader->shader);
}

static int shader_compile(GLuint shader, const char *code, char *error_buffer, size_t error_buffer_capacity)
{
    gl_shader_source(shader, 1, (const char **)&code, 0);
    gl_compile_shader(shader);

    GLint result = GL_FALSE;

    gl_get_shaderiv(shader, GL_COMPILE_STATUS, &result);

    if(result == GL_TRUE)
    {
        GLint log_size = 0;

		if (error_buffer && error_buffer_capacity > 0) 
        {
			gl_get_shaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
			gl_get_shader_info_log(shader, error_buffer_capacity, 0, error_buffer);

            error_buffer[error_buffer_capacity - 1] = '\0';

			return KEK_ERROR;
		}
    }

    return KEK_OK;
}

