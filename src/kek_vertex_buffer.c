#include "kek.h"
#include "pal.h"
#include <stdbool.h>
#include <stdlib.h>

static MemPool pool;
static int vertex_buffer_write(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size);

void vertex_buffer_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(VertexBuffer));
}

VertexBuffer *vertex_buffer_create(size_t capacity)
{
    //todo: use a pool
     VertexBuffer *inst = mem_pool_take(&pool);

    if(inst == NULL)
        return NULL;

    inst->capacity = capacity;
    inst->size = 0;

    inst->map_buffer = NULL;
    gl_gen_vertex_arrays(1, &inst->vao);
    gl_gen_buffers(1, &inst->vbo);
    gl_bind_buffer(GL_ARRAY_BUFFER, inst->vbo);
    gl_buffer_data(GL_ARRAY_BUFFER, capacity, NULL, GL_DYNAMIC_DRAW);

    return inst;
}

void vertex_buffer_destroy(VertexBuffer *vb)
{
    gl_delete_buffers(1, &vb->vbo);
    gl_delete_vertex_arrays(1, &vb->vao);

    vb->vao = 0;
    vb->vbo = 0;
    vb->map_buffer = NULL;

    mem_pool_release(&pool, vb);
}

size_t vertex_buffer_get_capacity(VertexBuffer *vb)
{
    return vb->capacity;
}

size_t vertex_buffer_get_size(VertexBuffer *vb)
{
    return vb->size;
}

void vertex_buffer_clear(VertexBuffer *vb)
{
    vb->size = 0;
}

void vertex_buffer_set_attribs(VertexBuffer *vb, size_t *attribs, size_t count)
{
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);

	size_t stride = 0;

	for (size_t i = 0; i < count; ++i) 
		stride += sizeof(float) * attribs[i];

	intptr_t byte_offset = 0;

	for (size_t i = 0; i < count; ++i) {
		
		int float_size = attribs[i];

		gl_enable_vertex_attrib_array(i);
		gl_vertex_attrib_pointer(i, float_size, GL_FLOAT, GL_FALSE, stride, (void *)byte_offset);

		byte_offset += sizeof(float) * float_size;
	}
}

int vertex_buffer_fill(VertexBuffer *vb, uint8_t *data, size_t size)
{
    PALReturn perr = PAL_OK;

    perr = vertex_buffer_map(vb);

    if(perr == PAL_OK)
    {
        perr = vertex_buffer_write(vb, 0, data, size);

        vertex_buffer_unmap(vb);
    }

    if(perr == PAL_OK)
        vb->size = size;
    else
        vb->size = 0;

    return (perr == PAL_OK) ? KEK_OK : KEK_ERROR;
}

void vertex_buffer_bind(VertexBuffer *vb)
{
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
}

static void *mapped_buffer = false;
int vertex_buffer_map(VertexBuffer *vb)
{
    if(mapped_buffer != NULL)
        return KEK_ERROR;

    if(vb->map_buffer != NULL)
        return KEK_ERROR;

	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);

	mapped_buffer = (char *)gl_map_buffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vb->map_buffer = mapped_buffer;

    return PAL_OK;
}

static int vertex_buffer_write(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size)
{
    if(vb->map_buffer == NULL)
        return KEK_ERROR;

    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

    uint8_t *addr = (uint8_t *)vb->map_buffer;

	memcpy(&addr[offset], data, size);

    return PAL_OK;
}

int vertex_buffer_unmap(VertexBuffer *vb)
{
    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

	gl_unmap_buffer(GL_ARRAY_BUFFER);

    mapped_buffer = NULL;
    vb->map_buffer = NULL;
}

int vertex_buffer_append(VertexBuffer *vb, uint8_t *data, size_t size_bytes)
{
    return vertex_buffer_write(vb, vb->size, data, size_bytes);
    vb->size += size_bytes;
}

void vertex_buffer_draw(VertexBuffer *vb, size_t start, size_t count)
{
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_TRIANGLES, start, count);
}
