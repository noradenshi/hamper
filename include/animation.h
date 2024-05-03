#pragma once
#include <raylib.h>

typedef struct _Frame Frame;
typedef struct _Animation Animation;

extern struct _Animations {
    Animation *hamster_idle;
    Animation *hamster_walk;
    Animation *alley_idle;
} animations;

void animationInit();
Rectangle *animationGetFrame(Animation *animation);
void animationSetFlipped(Animation *animation, bool is_flipped);
void animationReset(Animation *animation);
void animationUpdate(Animation *animation);
void animationUnload();
