#include "resources.h"
#include "animation.h"
#include <raylib.h>
#include <stdlib.h>

struct _WindowData window_data = {0};
struct _Textures textures = {0};
struct _Recs recs = {0};

void resourcesInit() {
    animationInit();

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
    textures.cursor = LoadTexture("resources/cursor.png");
}

void windowSizeUpdate() {
    window_data.WIDTH = GetScreenWidth();
    window_data.HEIGHT = GetScreenHeight();
    window_data.CENTER.x = window_data.WIDTH / 2.f;
    window_data.CENTER.y = window_data.HEIGHT / 2.f;
}

void resourcesUnload() {
    animationUnload();

    UnloadTexture(textures.tileset);
    UnloadTexture(textures.hamster);
    UnloadTexture(textures.cursor);
    free(recs.tileset);
}
