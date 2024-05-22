#pragma once
#include <raylib.h>

typedef struct _Tile Tile;
typedef struct _Tilemap Tilemap;

typedef struct _Collisions {
    Rectangle rec_x;
    Rectangle rec_y;
} Collisions;

Tilemap *tilemapEmpty();
Tilemap *tilemapLoad(const char *filename);
Collisions *tilemapGetCollisions(Tilemap *tilemap, Rectangle rectangle);
void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id);
void tilemapSave(Tilemap *tilemap, const char *filename);
void tilemapDraw(Tilemap *tilemap);
void tilemapUnload(Tilemap *tilemap);
