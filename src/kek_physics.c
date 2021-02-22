#include "kek.h"
#include <stdlib.h>

typedef struct collision {
    int entity_a;
    int entity_b;
} Collision;

static Collision *stack_head = NULL;
static size_t stack_count = 0; 

static void physics_query_cb(int entity_a, void *ctx);
static bool circle_circle(int entity_a, int entity_b);
static bool rect_circle(int entity_a, int entity_b);
static bool rect_rect(int entity_a, int entity_b);
static int collision_sort(const void *a, const void *b);

void init_physics(void)
{
}
void simulate_physics(int sceneid)
{

    Scene *scene = get_scene(sceneid);

    Entity *entity = scene->entities;

    stack_head = NULL;
    stack_count = 0;

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
        entity_position(entityid, add_vec3(position, velocity));
        
        entity = entity->scene_next_entity;
    }

    qsort(stack_head, stack_count, sizeof(Collision), collision_sort);

    // narrow phase  
    Collision *last = NULL;
    for(size_t i = 0; i < stack_count; ++i)
    {
        Collision *c = &stack_head[i];

        bool collision = false;

        Collider *ca = get_entity_collider(c->entity_a);
        Collider *cb = get_entity_collider(c->entity_b);

        if(last && c->entity_a == last->entity_a && c->entity_b == c->entity_b)
            continue;

        if(ca->type == COLLIDER_CIRCLE && cb->type == COLLIDER_CIRCLE)
            collision = circle_circle(c->entity_a, c->entity_b);

        else if(ca->type == COLLIDER_RECT && cb->type == COLLIDER_CIRCLE)
            collision = rect_circle(c->entity_a, c->entity_b);

        else if(cb->type == COLLIDER_RECT && ca->type == COLLIDER_CIRCLE)
            collision = rect_circle(c->entity_b, c->entity_a);

        else if(ca->type == COLLIDER_RECT && cb->type == COLLIDER_RECT)
            collision = rect_rect(c->entity_a, c->entity_b);

        if(collision)
        {
            if(ca->collision_fn)
                ca->collision_fn(c->entity_a, c->entity_b, ca->ctx);

            if(cb->collision_fn)
                cb->collision_fn(c->entity_b, c->entity_a, cb->ctx);
        }

        last = c;
    }


    if(stack_head)
    {
        memstack_pop(stack_head);
        stack_head = NULL;
        stack_count = 0;
    }

}


static void physics_query_cb(int entity_a, void *ctx)
{
    int entity_b = *(int*)ctx;

    Collider *ca = get_entity_collider(entity_a);
    Collider *cb = get_entity_collider(entity_b);

    if(entity_a == entity_b)
        return;

    if(!ca || !cb)
        return;

    if((ca->mask & cb->category) == 0 && (ca->category & cb->mask) == 0)
        return;

    Collision *c = memstack_push(sizeof(Collision));
    c->entity_a = KEK_MAX(entity_a, entity_b);
    c->entity_b = KEK_MIN(entity_a, entity_b);
    ++stack_count;

    if(!stack_head)
        stack_head = c;

}

static bool rect_circle(int entity_a, int entity_b)
{
    Vec3 pos_a = get_entity_position(entity_a);
    Vec3 pos_b = get_entity_position(entity_b);
    Collider *ca = get_entity_collider(entity_a);
    Collider *cb = get_entity_collider(entity_b);
    Vec2 ah = mul_vec2_f(ca->rect, 0.5);

    Vec2 corners[4] = {
        sub_vec2(pos_a.xy, vec2(-ah.x, -ah.y)),
        sub_vec2(pos_a.xy, vec2( ah.x, -ah.y)),
        sub_vec2(pos_a.xy, vec2( ah.x,  ah.y)),
        sub_vec2(pos_a.xy, vec2(-ah.x,  ah.y))
    };
    Vec2 lines[4][2] = {
        {corners[0], corners[1]},
        {corners[1], corners[2]},
        {corners[2], corners[3]},
        {corners[3], corners[0]}
    };

    // check overlapping corners
    for(size_t i = 0; i < 4; ++i)
    {
        float d2 = length2_vec2(sub_vec2(pos_b.xy, corners[i]));
        float r2 = cb->radius * cb->radius;

        if(d2 < r2)
            return true;
    }
    
    // check overlapping edges
    for(size_t i = 0; i < 4; ++i)
    {
        Vec2 p0 = lines[i][0];
        Vec2 p1 = lines[i][1];
        Vec2 c = pos_b.xy;
        float r = cb->radius;
        Vec2 intersect;
        if(line_circle_collision(p0, p1, c, r, &intersect))
            return true;
    }

    return false;
}

static bool circle_circle(int entity_a, int entity_b)
{
    bool jerk = false;

    Vec3 pos_a = get_entity_position(entity_a);
    Vec3 pos_b = get_entity_position(entity_b);

    Collider *ca = get_entity_collider(entity_a);
    Collider *cb = get_entity_collider(entity_b);

    float rd2 = ca->radius * ca->radius + cb->radius * cb->radius;
    float pd2 = length2_vec2(sub_vec2(pos_b.xy, pos_a.xy));

    return (pd2 <= rd2);
}

static bool rect_rect(int entity_a, int entity_b)
{
    Vec3 p0 = get_entity_position(entity_a);
    Vec3 p1 = get_entity_position(entity_b);
    Vec3 s0 = get_entity_size(entity_a);
    Vec3 s1 = get_entity_size(entity_b);

   return aabb(p0.xy, s0.xy, p1.xy, s1.xy);
}

static int collision_sort(const void *a, const void *b)
{
    const Collision *ca = (Collision *)a;
    const Collision *cb = (Collision *)b;

    int major = ca->entity_a - cb->entity_b;
    int minor = ca->entity_a - cb->entity_b;

    if(major != 0)
        return major;

    return minor;
}

