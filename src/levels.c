#include "levels.h"
#include "hamster.h"
#include "item.h"
#include "tilemap.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

struct LevelInfo {
    Tilemap *tilemap;
    Item *items;
    int item_count;
    int item_capacity;
} levels[LEVEL_SIZE];

const char *tilemap_filenames[LEVEL_SIZE] = {"resources/levels/test_menu",
                                             "resources/levels/test_tmp"};
void levelSpawnItem(Level level, ItemType type, Vector2 position) {
    if (levels[level].item_count == levels[level].item_capacity) {
        levels[level].item_capacity *= 2;
        levels[level].item_capacity += 1;
        levels[level].items =
            realloc(levels[level].items, sizeof(levels[level].items[0]) *
                                             levels[level].item_capacity);
    }

    itemSpawn(&levels[level].items[levels[level].item_count], type, position);
    levels[level].item_count++;
}

void levelsLoad() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        levels[i].tilemap = tilemapLoad(tilemap_filenames[i]);
        levels[i].item_count = 0;
        levels[i].item_capacity = 0;
        levels[i].items = NULL;
    }

    levelSpawnItem(LEVEL_TMP, ITEM_LINE, (Vector2){20, 130});
}

Tilemap *levelGetTilemap(Level level) { return levels[level].tilemap; }

const char *levelsFilename(Level level) { return tilemap_filenames[level]; }

void levelUpdateItems(Level level) {
    for (int i = 0; i < levels[level].item_count; i++) {
        if (levels[level].items[i].type != ITEM_NONE) {
            if (CheckCollisionRecs(levels[level].items[i].position_rec, hamsterGetRect())) {
                levels[level].items[i].on_pickup();
                levels[level].items[i].type = ITEM_NONE;
            }
        }
    }
}

void levelDraw(Level level) {
    tilemapDraw(levels[level].tilemap);
    for (int i = 0; i < levels[level].item_count; i++) {
        if (levels[level].items[i].type != ITEM_NONE)
            itemDraw(&levels[level].items[i]);
    }
}

void levelsUnload() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        tilemapUnload(levels[i].tilemap);
    }
}
