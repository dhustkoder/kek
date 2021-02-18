#include "kek.h"

static void physics_query_cb(int entity_a, void *ctx);

void init_physics(void)
{
}

void simulate_physics(int sceneid)
{
    Scene *scene = get_scene(sceneid);

    Entity *entity = scene->entities;
    while(entity)
    {
        int entityid = entity->id;
        Vec3 position = get_entity_position(entityid);
        Vec3 velocity = get_entity_velocity(entityid);
        Vec3 size = get_entity_size(entityid);

        Vec2 p0;
        Vec2 p1;
        p0.x = position.x - size.x * 0.5f;
        p0.y = position.y - size.y * 0.5f;
        p1.x = position.x + size.x * 0.5f;
        p1.y = position.y + size.y * 0.5f;

        query_entity(p0, p1, physics_query_cb, &entityid); 

        entity = entity->scene_next_entity;
    }

    // foreach node
    // query collision
    //  if collide, put on collision stack
    //  sort by min_max id
   
    // iterate:
    //  if duplicate from previous, skip
    //  else: update

}


static void physics_query_cb(int entity_a, void *ctx)
{
    int entity_b = *(int*)ctx;

    Vec3 p0 = get_entity_position(entity_a);
    Vec3 s0 = get_entity_size(entity_a);
    Vec3 p1 = get_entity_position(entity_b);
    Vec3 s1 = get_entity_size(entity_b);

    if(entity_a == entity_b)
        return;

    if(!aabb(p0.xy, s0.xy, p1.xy, s1.xy))
        return;


}
