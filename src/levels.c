#include "levels.h"
#include "entities/entity.h"
#include "entities/hamster.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>

struct _LevelData {
    Tilemap *tilemap;
    Entity *entities;
    int entity_count;
    int entity_capacity;
    float gravity;
} levels[LEVEL_SIZE];

const char *tilemap_core_paths[LEVEL_SIZE] = {"resources/levels/core/test_menu",
                                              "resources/levels/core/test_tmp"};

void levelSpawnEntity(LevelData *level, EntityType type, Vector2 position) {
    if (level->entity_count == level->entity_capacity) {
        level->entity_capacity *= 2;
        level->entity_capacity += 1;
        level->entities = realloc(level->entities, sizeof(level->entities[0]) *
                                                       level->entity_capacity);
    }

    entitySpawn(&level->entities[level->entity_count], type, position);
    level->entity_count++;
}

LevelData *levelLoad(const char *filename) {
    Tilemap *tilemap = tilemapLoad(filename);
    LevelData *level = malloc(tilemapGetSize(tilemap) + sizeof(LevelData));
    level->tilemap = tilemap;
    level->entity_count = 0;
    level->entity_capacity = 0;
    level->entities = NULL;
    level->gravity = 8.f;

    return level;
}

float levelGetGravity(LevelData *level) { return level->gravity; }

Tilemap *levelGetTilemap(LevelData *level) { return level->tilemap; }

const char *levelCorePath(Level level) { return tilemap_core_paths[level]; }

void levelHandleCollisions(LevelData *level) {
    Collisions *collisions =
        tilemapGetCollisions(level->tilemap, *hamsterGetRect());
    hamsterHandleCollisions(collisions);
}

void levelUpdateEntities(LevelData *level) {
    for (int i = 0; i < level->entity_count; i++) {
        if (level->entities[i].type != ENTITY_NONE &&
            !level->entities[i].is_stationary) {
            if (CheckCollisionRecs(level->entities[i].position_rec,
                                   *hamsterGetRect())) {
                // level->entities[i].on_pickup();
                // level->entities[i].type = ENTITY_NONE;
                level->entities[i].position_rec.x +=
                    level->entities[i].position_rec.x - hamsterGetRect()->x;
            }
        }
    }
}

void levelDraw(LevelData *level) {
    tilemapDraw(level->tilemap);
    for (int i = 0; i < level->entity_count; i++) {
        if (level->entities[i].type != ENTITY_NONE)
            entityDraw(&level->entities[i]);
    }
}

void levelUnload(LevelData *level) {
    if (level == nullptr)
        return;

    if (level->entities == nullptr)
        free(level->entities);

    tilemapUnload(level->tilemap);
    free(level);
}
