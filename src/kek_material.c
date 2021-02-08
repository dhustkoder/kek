#include "kek.h"
#include <assert.h>

#if 0
static MemPool pool;

void material_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Material));
}

Material *material_create(void)
{
    Material *material = mem_pool_take(&pool);

    material->count = 0;
}

void material_add_property(Material *material, const MaterialProperty *property)
{
    assert(material->count < MATERIAL_PROPERTY_CAPACITY);

    material->properties[material->count] = *property;
    ++material->count;
}

void material_destroy(void)
{
    mem_pool_release(&pool, material);
}
#endif
