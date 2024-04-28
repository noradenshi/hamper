#include "resources.h"
#include "animation.h"
#include <raylib.h>
#include <stdlib.h>

struct _WindowData window_data = {0};
struct _Textures textures = {0};
struct _Recs recs = {0};
struct _Frames frames = {0};
struct _Animations animations = {0};

void resourcesInit() {
    textures.tileset = LoadTexture("resources/terrain.png");
    recs.tileset = malloc(sizeof(Rectangle) * textures.tileset.width *
                          textures.tileset.height);
    if (!recs.tileset) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    for (int i = 0; i < textures.tileset.width * textures.tileset.height; i++) {
        recs.tileset[i].width = TILE_SIZE;
        recs.tileset[i].height = TILE_SIZE;
        recs.tileset[i].x = i % textures.tileset.width * recs.tileset[i].width;
        recs.tileset[i].y =
            (int)(i / textures.tileset.width) * recs.tileset[i].height;
    }

    textures.background = LoadTexture("resources/background.png");
    textures.hamster = LoadTexture("resources/hamster.png");
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

void windowSizeUpdate() {
    window_data.WIDTH = GetScreenWidth();
    window_data.HEIGHT = GetScreenHeight();
    window_data.CENTER.x = window_data.WIDTH / 2.f;
    window_data.CENTER.y = window_data.HEIGHT / 2.f;
}

void resourcesUnload() {
    UnloadTexture(textures.tileset);
    UnloadTexture(textures.hamster);
    animationUnload(animations.hamster_walk);
    animationUnload(animations.hamster_idle);
    free(recs.tileset);
}
