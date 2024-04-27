#include "tilemap.h"
#include "resources.h"
#include <errno.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const Vector2 tile_origin = {0};

// private
int tilemapGetTileAt(Tilemap *tilemap, Vector2 point) {
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

void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id) {
    int tile_id = tilemapGetTileAt(tilemap, (Vector2){pos.x, pos.y});
    if (tile_id == -1) {
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
}

void tilemapUnload(Tilemap *tilemap) {
    free(tilemap->tiles);
    free(tilemap);
}
