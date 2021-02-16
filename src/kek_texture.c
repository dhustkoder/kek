#include "kek.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"


static MemPool pool;
void init_texture(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Texture));
}

int create_texture(void)
{
     Texture *inst = mempool_take(&pool);
     inst->id = mempool_get_slot(&pool, inst);
     inst->width = 0;
     inst->height = 0;
     inst->loaded = false;

     gl_gen_textures(1, &inst->glid);

     return inst->id;
}

Texture *get_texture(int id)
{
    return mempool_get_addr(&pool, id);
}

void destroy_texture(int id)
{
    Texture *texture = mempool_get_addr(&pool, id);

    gl_delete_textures(1, &texture->glid);

    mempool_release(&pool, texture);
}

int load_texture_file(int id, const char *file)
{
	int w;
	int h;
	int channels;

    Texture *texture = mempool_get_addr(&pool, id);

    stbi_set_flip_vertically_on_load(1);
	unsigned char *image = stbi_load(file, &w, &h, &channels, STBI_rgb_alpha);

	if (!image)
		return KEK_ERROR;

	gl_bind_texture(GL_TEXTURE_2D, texture->glid);
	//gl_tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//gl_tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl_tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl_tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl_tex_image_2d(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	texture->width = w;
	texture->height = h;

    texture->loaded = true;

    return KEK_OK;
}

void bind_texture(int id, int slot)
{
    Texture *texture = mempool_get_addr(&pool, id);

    gl_active_texture(GL_TEXTURE0 + slot);
    gl_bind_texture(GL_TEXTURE_2D, texture->glid);

}

