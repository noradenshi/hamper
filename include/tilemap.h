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
const char *tilemapGetPath(Tilemap *filename);
void tilemapSetPath(Tilemap *tilemap, const char *filename);
int tilemapGetSize(Tilemap *tilemap);
Collisions *tilemapGetCollisions(Tilemap *tilemap, Rectangle rectangle);
void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id);
void tilemapSave(Tilemap *tilemap);
void tilemapDraw(Tilemap *tilemap);
void tilemapUnload(Tilemap *tilemap);
