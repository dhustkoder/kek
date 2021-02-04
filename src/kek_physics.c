#include "kek.h"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_structs.h"

#define cp_body_new cpBodyNew
#define cp_moment_for_circle cpMomentForCircle
#define cp_body_get_angle cpBodyGetAngle
#define cp_body_set_angle cpBodySetAngle
#define cp_body_set_torque cpBodySetTorque
#define cp_body_set_mass cpBodySetMass
#define cp_shape_filter_new cpShapeFilterNew
#define cp_shape_set_filter cpShapeSetFilter
#define cp_circle_shape_new cpCircleShapeNew
#define cp_space_bb_query cpSpaceBBQuery
#define cp_space_add_collision_handler cpSpaceAddstruct collision_handler
#define cp_space_add_constraint cpSpaceAddConstraint
#define cp_space_add_shape cpSpaceAddShape
#define cp_space_add_body cpSpaceAddBody
#define cp_body_set_force cpBodySetForce
#define cp_bb_new cpBBNew
#define cp_rotary_limit_joint_new cpRotaryLimitJointNew
#define cp_pin_joint_new cpPinJointNew
#define cp_pivot_joint_new2 cpPivotJointNew2
#define cp_damped_spring_new cpDampedSpringNew
#define cp_pin_joint_set_dist cpPinJointSetDist

typedef void (*EntityQueryFn)(struct entity *e, void *ctx);

struct entity_query_context
{
    EntityQueryFn fn;
    void *user_data;
};

struct collision_handler
{
    struct collision_handler *handler;
    uint32_t category_a;
    uint32_t category_b;
    void *ctx;
    KEKCollisionFn fn;
};

#define MAX_RENDER_SHAPES 1024
#define MAX_COLLIDE_HANDLERS 1024 

static cpSpace *space;
static cpShape *render_shapes[MAX_RENDER_SHAPES];
static int32_t render_shape_count = 0;
static struct collision_handler collide_handlers[MAX_COLLIDE_HANDLERS];
static int32_t collide_handler_count = 0;

void physics_init(void)
{
    space = cpSpaceNew();
    space->iterations = 10;
    space->gravity = cpv(0, 0);
}

void physics_simulate(void)
{
    cpSpaceStep(space, 1.f/60.f);
}

#if 0
void physics_create_circle(struct entity *entity, union vec3 position, float radius, uint32_t type)
{
    cpFloat moment = cp_moment_for_circle(0.1f, 0.0f, radius, cpvzero);
    cpBody *body = cp_body_new(10.f, moment);

    entity->body = cp_space_add_body(space, body);
    entity->body->p = cpv(position.x, position.y);
    entity->body->v = cpvzero;

    cpShape *shape = cp_circle_shape_new(e->body, radius, cpvzero);
    entity->shape = (cpShape *)cp_space_add_shape(space, (cpShape *)shape);
    entity->shape->type = type;
	entity->shape->e = 0.0f; 
    entity->shape->u = 0.9f;
    entity->shape->userData = e;
}
#endif

struct physics_body *physics_create_polygon_shape(union vec3 *positions, size_t count, uint32_t type)
{
    return NULL;
}

void physics_destroy_body(struct physics_body *body)
{
}

struct physics_constraint *physics_add_damped_spring(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b, float rest_length, float stiffness, float damping)
{
    return NULL;
}
        
struct physics_constraint *physics_add_pin_joint(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b, float dist)
{
    return NULL;
}

struct physics_constraint *physics_add_pivot_joint(struct physics_body *a, struct physics_body *b, union vec2 anchor_a, union vec2 anchor_b)
{
    return NULL;
}

struct physics_constraint *physics_add_rotary_limit_joint(struct physics_body *a, struct physics_body *b, float min, float max)
{
    return NULL;
}

void physics_set_filter(struct physics_body *e, uint32_t group, uint32_t category, uint32_t mask)
{
}

void physics_set_mass(struct physics_body *e, float mass)
{
}

void physics_set_force(struct physics_body *e, union vec2 force)
{
}

void physics_set_torque(struct physics_body *e, float torque)
{
}

void physics_set_angle(struct physics_body *e, float angle)
{
}

void physics_body_set_position(struct physics_body *e, union vec2 pos)
{
}

union vec2 physics_body_get_position(struct physics_body *e)
{
    return vec2(0,0);
}

float physics_body_get_angle(struct physics_body *e)
{
    return 0.0;
}

union vec2 physics_get_center(struct physics_body *e)
{
    return vec2(0,0);
}

