#pragma once
#include "animation.h"
#include <raylib.h>

#define TILE_SIZE 8
#define IDLE_SIZE 5
#define WALK_SIZE 8

extern struct _WindowData {
    int WIDTH;
    int HEIGHT;
    Vector2 CENTER;
} window_data;

extern struct _Textures {
    Texture tileset;
    Texture hamster;
    Texture background;
} textures;

extern struct _Recs {
    Rectangle *tileset;
} recs;

extern struct _Frames {
    Frame hamster_idle[IDLE_SIZE];
    Frame hamster_walk[WALK_SIZE];
} frames;

extern struct _Animations {
    Animation *hamster_walk;
    Animation *hamster_idle;
} animations;

void resourcesInit();
void windowSizeUpdate();
void resourcesUnload();
