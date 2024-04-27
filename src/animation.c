#include <animation.h>
#include <raylib.h>
#include <stdlib.h>

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

void animationUnload(Animation *animation) {
    free(animation->frames);
    free(animation);
}
