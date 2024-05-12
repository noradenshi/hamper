#pragma once
#include <raylib.h>

#define TILE_SIZE 8

extern struct _WindowData {
    int WIDTH;
    int HEIGHT;
    Vector2 CENTER;
} window_data;

extern struct _Textures {
    Texture tileset;
    Texture players;
    Texture cursor;
    Texture background;
} textures;

extern struct _Recs {
    Rectangle *tileset;
} recs;

extern struct _Sounds {
    Sound walk_grass;
    Sound draw1;
    Sound draw2;
} sounds;

void resourcesInit();
void windowSizeUpdate();
void resourcesUnload();
