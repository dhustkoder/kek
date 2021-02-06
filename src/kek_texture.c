#include "kek.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"


static MemPool pool;
void texture_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Texture));
}

Texture *texture_create(void)
{
     Texture *inst = mem_pool_take(&pool);
     inst->width = 0;
     inst->height = 0;
     inst->loaded = false;

     gl_gen_textures(1, &inst->id);

     return inst;
}

void texture_destroy(Texture *texture)
{

    gl_delete_textures(1, &texture->id);

    mem_pool_release(&pool, texture);
}

int texture_load_file(Texture *texture, const char *file)
{
	int w;
	int h;
	int channels;

    stbi_set_flip_vertically_on_load(1);
	unsigned char *image = stbi_load(file, &w, &h, &channels, STBI_rgb_alpha);

	if (!image)
		return KEK_ERROR;

	gl_bind_texture(GL_TEXTURE_2D, texture->id);
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

void texture_bind(Texture *texture, int slot)
{
    gl_active_texture(GL_TEXTURE0 + slot);
    gl_bind_texture(GL_TEXTURE_2D, texture->id);
}

