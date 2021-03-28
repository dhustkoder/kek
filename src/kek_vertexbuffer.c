#include "kek.h"
#include "pal.h"
#include <stdbool.h>
#include <stdlib.h>

static MemPool pool;
static VertexBuffer *root = NULL;
static int write_vertexbuffer(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size);

void init_vertexbuffer(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(VertexBuffer));
    root = NULL;

}

VertexBuffer *get_vertexbuffer(int id)
{
    return mempool_get_addr(&pool, id);
}

int create_vertexbuffer(size_t capacity)
{
    //todo: use a pool
     VertexBuffer *inst = mempool_take(&pool);

    if(inst == NULL)
        return -1;

    inst->id = mempool_get_slot(&pool, inst);
    inst->capacity = capacity;
    inst->size = 0;

    inst->map_buffer = NULL;
    gl_gen_vertex_arrays(1, &inst->vao);
    gl_gen_buffers(1, &inst->vbo);
    gl_bind_buffer(GL_ARRAY_BUFFER, inst->vbo);
    gl_buffer_data(GL_ARRAY_BUFFER, capacity, NULL, GL_DYNAMIC_DRAW);

    inst->next = root;
    root = inst;

    return inst->id;
}

void destroy_vertexbuffer(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    gl_delete_buffers(1, &vb->vbo);
    gl_delete_vertex_arrays(1, &vb->vao);

    vb->vao = 0;
    vb->vbo = 0;
    vb->map_buffer = NULL;

    if(root == vb)
    {
        root  = vb->next;
    }
    else
    {
        VertexBuffer *last = root;
        VertexBuffer *node = root->next;

        while(node)
        {
            if(node == vb)
            {
                last->next = vb->next;
                break;
            }

            last = node;
            node = node->next;
        }
    }
        
    mempool_release(&pool, vb);
}

//todo: not called or used
void flush_dynamic_vertexbuffers(void)
{
    VertexBuffer *node = root;

    while(node)
    {
        clear_vertexbuffer(node->id);
        node = node->next;
    }
}

size_t get_vertexbuffer_capacity(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    return vb->capacity;
}

size_t get_vertexbuffer_size(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    return vb->size;
}

void clear_vertexbuffer(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    vb->size = 0;
}

void vertexbuffer_attribs(int vbid, const size_t *attribs, size_t count)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);

	size_t stride = 0;

	for (size_t i = 0; i < count; ++i) 
		stride += sizeof(float) * attribs[i];

	intptr_t byte_offset = 0;

	for (size_t i = 0; i < count; ++i) {
		
		size_t float_size = attribs[i];

		gl_enable_vertex_attrib_array((GLuint)i);
		gl_vertex_attrib_pointer((GLuint)i, (GLint)float_size, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void *)byte_offset);

		byte_offset += sizeof(float) * float_size;
	}
}

int fill_vertexbuffer(int vbid, uint8_t *data, size_t size)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);

    int perr = map_vertexbuffer(vbid);

    if(perr == KEK_OK)
    {
        perr = write_vertexbuffer(vb, 0, data, size);

        unmap_vertexbuffer(vbid);
    }

    if(perr == KEK_OK)
        vb->size = size;
    else
        vb->size = 0;

    return perr;
}

void bind_vertexbuffer(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
}

static void *mapped_buffer = false;
int map_vertexbuffer(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    if(mapped_buffer != NULL)
        return KEK_ERROR;

    if(vb->map_buffer != NULL)
        return KEK_ERROR;

	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);

	mapped_buffer = (char *)gl_map_buffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vb->map_buffer = mapped_buffer;

    return KEK_OK;
}

static int write_vertexbuffer(VertexBuffer *vb, size_t offset, uint8_t *data, size_t size)
{
    if(vb->map_buffer == NULL)
        return KEK_ERROR;

    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

    uint8_t *addr = (uint8_t *)vb->map_buffer;

	memcpy(&addr[offset], data, size);

    return KEK_OK;
}

int unmap_vertexbuffer(int vbid)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    if(mapped_buffer != vb->map_buffer)
        return KEK_ERROR;

	gl_unmap_buffer(GL_ARRAY_BUFFER);

    mapped_buffer = NULL;
    vb->map_buffer = NULL;
    return KEK_OK;
}

void append_vertexbuffer(int vbid, uint8_t *data, size_t size_bytes)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
    write_vertexbuffer(vb, vb->size, data, size_bytes);
    vb->size += size_bytes;
}

void draw_vertexbuffer(int vbid, size_t start, size_t count)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_TRIANGLES, (GLint)start, (GLsizei)count);
}

void draw_vertexbuffer_lines(int vbid, size_t start, size_t count)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
    gl_draw_arrays(GL_LINES, (GLint)start, (GLsizei)count);
}

void draw_vertexbuffer_line_strip(int vbid, size_t start, size_t count)
{
    VertexBuffer *vb = get_vertexbuffer(vbid);
	gl_bind_vertex_array(vb->vao);
	gl_bind_buffer(GL_ARRAY_BUFFER, vb->vbo);
   gl_draw_arrays(GL_LINE_STRIP, (GLint)start, (GLsizei)count);
}
