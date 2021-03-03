#include "kek.h"
#include <assert.h>

static MemPool pool;

void init_animation(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Animation));
}

int create_animation(void)
{
    Animation *animation = mempool_take(&pool);

    animation->id = mempool_get_slot(&pool, animation);
    animation->frame_count = 0;
    animation->loop = 0;

    return animation->id;
}

Animation *get_animation(int animationid)
{
    return mempool_get_addr(&pool, animationid);
}

void destroy_animation(int animationid)
{
    Animation *animation = get_animation(animationid);
    mempool_release(&pool, animation);
}

void add_animation_frame(int animationid, AnimationFrame frame)
{
    Animation *animation = get_animation(animationid);
    assert(animation->frame_count < MAX_ANIMATION_FRAMES);

    animation->frames[animation->frame_count] = frame;

    animation->frame_count++;
}

void add_animation_frame_clip(int animationid, int textureid, int x, int y, int clip_width, int clip_height, float duration)
{
    Animation *animation = get_animation(animationid);

    AnimationFrame frame;
    Texture *texture = get_texture(textureid);

    // flip y
    y = texture->height - y - clip_height;
    
    frame.texture = textureid;
    frame.pixel_width = clip_width;
    frame.pixel_height = clip_height;
    frame.uv0.x = (float)x/(float)texture->width;
    frame.uv0.y = (float)y/(float)texture->height;
    frame.uv1.x = (float)(x + clip_width)/(float)texture->width;
    frame.uv1.y = (float)(y + clip_height)/(float)texture->height;
    frame.duration = duration;

    add_animation_frame(animationid, frame);
}

void animation_loop(int animationid, bool loop)
{
    Animation *animation = get_animation(animationid);
    animation->loop = loop;
}

