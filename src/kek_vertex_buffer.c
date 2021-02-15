#include "kek.h"
#include "pal.h"
#include <stdbool.h>
#include <stdlib.h>

static MemPool pool;
static int write_vertex_buffer(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size);

void init_vertex_buffer(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(VertexBuffer));
}

VertexBuffer *create_vertex_buffer(size_t capacity)
{
    //todo: use a pool
     VertexBuffer *inst = mempool_take(&pool);

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

void destroy_vertex_buffer(VertexBuffer *vb)
{
    gl_delete_buffers(1, &vb->vbo);
    gl_delete_vertex_arrays(1, &vb->vao);

    vb->vao = 0;
    vb->vbo = 0;
    vb->map_buffer = NULL;

    mempool_release(&pool, vb);
}

size_t get_vertex_buffer_capacity(VertexBuffer *vb)
{
    return vb->capacity;
}

size_t get_vertex_buffer_size(VertexBuffer *vb)
{
    return vb->size;
}

void clear_vertex_buffer(VertexBuffer *vb)
{
    vb->size = 0;
}

void vertex_buffer_attribs(VertexBuffer *vb, size_t *attribs, size_t count)
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

int fill_vertex_buffer(VertexBuffer *vb, uint8_t *data, size_t size)
{

    int perr = map_vertex_buffer(vb);

    if(perr == KEK_OK)
    {
        perr = write_vertex_buffer(vb, 0, data, size);

        unmap_vertex_buffer(vb);
    }

    if(perr == KEK_OK)
        vb->size = size;
    else
        vb->size = 0;

    return perr;
}

void bind_vertex_buffer(VertexBuffer *vb)
{
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
}

static void *mapped_buffer = false;
int map_vertex_buffer(VertexBuffer *vb)
{
    if(mapped_buffer != NULL)
        return KEK_ERROR;

    if(vb->map_buffer != NULL)
        return KEK_ERROR;

	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);

	mapped_buffer = (char *)gl_map_buffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vb->map_buffer = mapped_buffer;

    return KEK_OK;
}

static int write_vertex_buffer(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size)
{
    if(vb->map_buffer == NULL)
        return KEK_ERROR;

    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

    uint8_t *addr = (uint8_t *)vb->map_buffer;

	memcpy(&addr[offset], data, size);

    return KEK_OK;
}

int unmap_vertex_buffer(VertexBuffer *vb)
{
    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

	gl_unmap_buffer(GL_ARRAY_BUFFER);

    mapped_buffer = NULL;
    vb->map_buffer = NULL;
    return KEK_OK;
}

void append_vertex_buffer(VertexBuffer *vb, uint8_t *data, size_t size_bytes)
{
    write_vertex_buffer(vb, vb->size, data, size_bytes);
    vb->size += size_bytes;
}

void draw_vertex_buffer(VertexBuffer *vb, size_t start, size_t count)
{
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_TRIANGLES, start, count);
}

void draw_vertex_buffer_lines(VertexBuffer *vb, size_t start, size_t count)
{
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_LINES, start, count);
}

void draw_vertex_buffer_line_strip(VertexBuffer *vb, size_t start, size_t count)
{
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_LINE_STRIP, start, count);
}
