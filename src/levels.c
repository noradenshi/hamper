#include "levels.h"
#include "entities/entity.h"
#include "entities/hamster.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>

struct LevelInfo {
    Tilemap *tilemap;
    Entity *entities;
    int entity_count;
    int entity_capacity;
    float gravity;
} levels[LEVEL_SIZE];

const char *tilemap_filenames[LEVEL_SIZE] = {"resources/levels/test_menu",
                                             "resources/levels/test_tmp"};
void levelSpawnEntity(Level level, EntityType type, Vector2 position) {
    if (levels[level].entity_count == levels[level].entity_capacity) {
        levels[level].entity_capacity *= 2;
        levels[level].entity_capacity += 1;
        levels[level].entities =
            realloc(levels[level].entities, sizeof(levels[level].entities[0]) *
                                                levels[level].entity_capacity);
    }

    entitySpawn(&levels[level].entities[levels[level].entity_count], type,
                position);
    levels[level].entity_count++;
}

void levelsLoad() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        levels[i].tilemap = tilemapLoad(tilemap_filenames[i]);
        levels[i].entity_count = 0;
        levels[i].entity_capacity = 0;
        levels[i].entities = NULL;
    }

    levels[LEVEL_TMP].gravity = 8.f;

    levelSpawnEntity(LEVEL_TMP, ENTITY_BOX, (Vector2){20, 130});
}

float levelGetGravity(Level level) { return levels[level].gravity; }

Tilemap *levelGetTilemap(Level level) { return levels[level].tilemap; }

const char *levelsFilename(Level level) { return tilemap_filenames[level]; }

void levelHandleCollisions(Level level) {
    Collisions *collisions = tilemapGetCollisions(levels[level].tilemap, *hamsterGetRect());
    hamsterHandleCollisions(collisions);
}

void levelUpdateEntities(Level level) {
    for (int i = 0; i < levels[level].entity_count; i++) {
        if (levels[level].entities[i].type != ENTITY_NONE &&
            !levels[level].entities[i].is_stationary) {
            if (CheckCollisionRecs(levels[level].entities[i].position_rec,
                                   *hamsterGetRect())) {
                // levels[level].entities[i].on_pickup();
                // levels[level].entities[i].type = ENTITY_NONE;
                levels[level].entities[i].position_rec.x +=
                    levels[level].entities[i].position_rec.x -
                    hamsterGetRect()->x;
            }
        }
    }
}

void levelDraw(Level level) {
    tilemapDraw(levels[level].tilemap);
    for (int i = 0; i < levels[level].entity_count; i++) {
        if (levels[level].entities[i].type != ENTITY_NONE)
            entityDraw(&levels[level].entities[i]);
    }
}

void levelsUnload() {
    for (int i = 0; i < LEVEL_SIZE; i++) {
        tilemapUnload(levels[i].tilemap);
    }
}
