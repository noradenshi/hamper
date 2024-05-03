#include "tilemap.h"
#include "resources.h"
#include <errno.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct _Tile {
    Rectangle pos;
    int src_id;
};

struct _Tilemap {
    int capacity;
    int size;
    Tile *tiles;
};

Collisions collision_list;
const Vector2 tile_origin = {0};

int tilemapGetCollisionsSize() { return collision_list.size; }

// private
int tilemapGetTileIdAt(Tilemap *tilemap, Vector2 point) {
    for (int i = 0; i < tilemap->size; i++) {
        if (CheckCollisionPointRec(point, tilemap->tiles[i].pos)) {
            return i;
        }
    }
    return -1;
}

// public
Tilemap *tilemapEmpty() {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    if (!tilemap) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    tilemap->size = 0;
    tilemap->capacity = 64;
    tilemap->tiles = malloc(sizeof(Tile) * tilemap->capacity);
    if (!tilemap->tiles) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    return tilemap;
}

Tilemap *tilemapLoad(char *filename) {
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    if (!tilemap) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        TraceLog(LOG_ERROR, "Opening %s: %s", filename, strerror(errno));
        return NULL;
    }

    int size_to_load = 0;
    char tmp;
    while ((tmp = fgetc(fd)) != EOF) {
        if (tmp == '\n')
            size_to_load++;
    }
    rewind(fd);

    tilemap->size = 0;
    tilemap->capacity = size_to_load * 2;
    tilemap->tiles = malloc(sizeof(Tile) * tilemap->capacity);
    if (!tilemap->tiles) {
        TraceLog(LOG_ERROR, "Allocating memory failed!");
        abort();
    }

    for (int i = 0; i < tilemap->capacity; i++) {
        tilemap->tiles[i].src_id = -1;
    }

    const int buffer_size = 64;
    char buffer[buffer_size];
    while (fgets(buffer, buffer_size, fd)) {
        char *end;
        const float x = strtof(buffer, &end);
        const float y = strtof(end, &end);
        const int id = (int)strtof(end, NULL);

        tilemap->tiles[tilemap->size].pos =
            (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
        tilemap->tiles[tilemap->size].src_id = id;

        tilemap->size++;

        // prolly not needed in tilemapLoad()
        if (tilemap->size > tilemap->capacity) {
            TraceLog(LOG_ERROR, "tilemap full (capacity: %d)",
                     tilemap->capacity);
            fclose(fd);
            return NULL;
        }
    }

    fclose(fd);
    return tilemap;
}

// TODO: merge by longer sides, not position
Collisions *tilemapGetCollisions(Tilemap *tilemap, Rectangle rectangle) {
    collision_list.size = 0;
    for (int i = 0; i < tilemap->size; i++) {
        if (tilemap->tiles[i].src_id == -1)
            continue;

        Rectangle rect = GetCollisionRec(rectangle, tilemap->tiles[i].pos);
        if (rect.width > 3.f || rect.height > 3.f) {
            if (collision_list.size > 0) {
                if (rect.x == collision_list.rec[collision_list.size - 1].x) {
                    collision_list.rec[collision_list.size - 1].height +=
                        rect.height;
                    if (rect.y < collision_list.rec[collision_list.size - 1].y)
                        collision_list.rec[collision_list.size - 1].y = rect.y;

                    continue;
                }
                if (rect.y == collision_list.rec[collision_list.size - 1].y) {
                    collision_list.rec[collision_list.size - 1].width +=
                        rect.width;
                    if (rect.x < collision_list.rec[collision_list.size - 1].x)
                        collision_list.rec[collision_list.size - 1].x = rect.x;

                    continue;
                }
            }

            collision_list.rec[collision_list.size] = rect;
            collision_list.size++;
        }
    }
    return &collision_list;
}

void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id) {
    int tile_id = tilemapGetTileIdAt(tilemap, (Vector2){pos.x, pos.y});
    if (tile_id == -1) {
        if (src_id == -1)
            return;

        tile_id = tilemap->size;
        tilemap->size++;

        if (tilemap->size == tilemap->capacity) {
            Tile *p =
                realloc(tilemap->tiles, sizeof(Tile) * tilemap->capacity * 2);
            if (!p) {
                TraceLog(LOG_ERROR, "Couldn't reallocate memory! Adding more "
                                    "tiles is impossible.");
                return;
            }

            tilemap->tiles = p;
            tilemap->capacity *= 2;
            TraceLog(LOG_INFO, "Successfully reallocated memory!");
        }
    }
    tilemap->tiles[tile_id].pos = pos;
    tilemap->tiles[tile_id].src_id = src_id;
}

void tilemapSave(Tilemap *tilemap, char *filename) {
    FILE *fd = fopen(filename, "w");
    if (!fd) {
        TraceLog(LOG_ERROR, "Opening %s: %s", filename, strerror(errno));
        return;
    }

    for (int i = 0; i < tilemap->size; i++) {
        if (tilemap->tiles[i].src_id == -1)
            continue;

        fprintf(fd, "%d %d %d\n", (int)tilemap->tiles[i].pos.x,
                (int)tilemap->tiles[i].pos.y, tilemap->tiles[i].src_id);
    }

    fclose(fd);
}

void tilemapDraw(Tilemap *tilemap) {
    for (int i = 0; i < tilemap->size; i++) {
        if (tilemap->tiles[i].src_id == -1)
            continue;

        DrawTexturePro(textures.tileset, recs.tileset[tilemap->tiles[i].src_id],
                       tilemap->tiles[i].pos, tile_origin, 0.f, WHITE);
    }

    for (int i = 0; i < collision_list.size; i++) {
        DrawRectangleLinesEx(collision_list.rec[i], 1.f, RED);
    }
}

void tilemapUnload(Tilemap *tilemap) {
    free(tilemap->tiles);
    free(tilemap);
}
