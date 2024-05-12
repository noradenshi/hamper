#include "levels.h"

Tilemap *tilemaps[LEVEL_SIZE];

const char *tilemap_filenames[2] = {"resources/levels/test_menu",
                                    "resources/levels/test_tmp"};

void levelsLoad() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        tilemaps[i] = tilemapLoad(tilemap_filenames[i]);
    }
}

Tilemap *levelsGet(Level level) { return tilemaps[level]; }

const char *levelsFilename(Level level) { return tilemap_filenames[level]; }

void levelsUnload() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        tilemapUnload(tilemaps[i]);
    }
}
