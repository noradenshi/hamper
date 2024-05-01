#include <animation.h>
#include <raylib.h>
#include <stdlib.h>

#define IDLE_SIZE 5
#define WALK_SIZE 8

typedef struct _Frame {
    Rectangle rectangle;
    float delay;
    bool is_flipped;
} Frame;

struct _Frames {
    Frame hamster_idle[IDLE_SIZE];
    Frame hamster_walk[WALK_SIZE];
} frames;

struct _Animation {
    int size;
    int current_frame_id;
    float timer;
    bool is_flipped;
    Frame *frames;
};

struct _Animations animations = {0};

Animation *animationLoad(int size, Frame *frames);

void animationInit() {
    for (int i = 0; i < IDLE_SIZE; i++) {
        frames.hamster_idle[i].rectangle.x = i * 16;
        frames.hamster_idle[i].rectangle.y = 0;
        frames.hamster_idle[i].rectangle.width = 16;
        frames.hamster_idle[i].rectangle.height = 16;
        frames.hamster_idle[i].delay = .1f;
    }
    frames.hamster_idle[0].delay = 4.f; // main stance

    for (int i = 0; i < WALK_SIZE; i++) {
        frames.hamster_walk[i].rectangle.x = i * 16;
        frames.hamster_walk[i].rectangle.y = 16;
        frames.hamster_walk[i].rectangle.width = 16;
        frames.hamster_walk[i].rectangle.height = 16;
        frames.hamster_walk[i].delay = .1f;
    }

    animations.hamster_idle = animationLoad(IDLE_SIZE, frames.hamster_idle);
    animations.hamster_walk = animationLoad(WALK_SIZE, frames.hamster_walk);
}

Animation *animationLoad(int size, Frame *frames) {
    Animation *animation = calloc(1, sizeof(Animation));
    if (!animation) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    animation->current_frame_id = 0;
    animation->size = size;
    animation->frames = calloc(animation->size, sizeof(Frame));
    if (!animation->frames) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    for (int i = 0; i < size; i++) {
        animation->frames[i] = frames[i];
    }

    animation->timer = animation->frames[animation->current_frame_id].delay;
    return animation;
}

Rectangle *animationGetFrame(Animation *animation) {
    if (animation->is_flipped !=
        animation->frames[animation->current_frame_id].is_flipped) {
        animation->frames[animation->current_frame_id].is_flipped =
            !animation->frames[animation->current_frame_id].is_flipped;
        animation->frames[animation->current_frame_id].rectangle.width *= -1;
    }
    return &animation->frames[animation->current_frame_id].rectangle;
}

void animationSetFlipped(Animation *animation, bool is_flipped) {
    animation->is_flipped = is_flipped;
}

void animationReset(Animation *animation) {
    animation->current_frame_id = 0;
    animation->timer = animation->frames[0].delay;
}

void animationUpdate(Animation *animation) {
    animation->timer -= GetFrameTime();
    if (animation->timer > 0.f)
        return;

    animation->current_frame_id++;
    if (animation->current_frame_id >= animation->size)
        animation->current_frame_id = 0;

    animation->timer = animation->frames[animation->current_frame_id].delay;
}

void animationUnloadAnimation(Animation *animation) {
    free(animation->frames);
    free(animation);
}

void animationUnload() {
    animationUnloadAnimation(animations.hamster_idle);
    animationUnloadAnimation(animations.hamster_walk);
}
