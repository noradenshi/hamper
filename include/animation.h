#pragma once
#include <raylib.h>

typedef struct _Frame {
    Rectangle rectangle;
    float delay;
    bool is_flipped;
} Frame;

typedef struct _Animation {
    int size;
    int current_frame_id;
    float timer;
    bool is_flipped;
    Frame *frames;
} Animation;

Animation *animationLoad(int size, Frame *frames);
Rectangle *animationGetFrame(Animation *animation);
void animationReset(Animation *animation);
void animationUpdate(Animation *animation);
void animationUnload(Animation *animation);
