#pragma once
#include <raylib.h>

typedef struct _Tile Tile;
typedef struct _Tilemap Tilemap;

#define MAX_COLLISIONS 6
typedef struct _Collisions {
    Rectangle rec[MAX_COLLISIONS];
    int size;
} Collisions;

// DEBUG
int tilemapGetCollisionsSize();

Tilemap *tilemapEmpty();
Tilemap *tilemapLoad(char *filename);
Collisions *tilemapGetCollisions(Tilemap *tilemap, Rectangle rectangle);
void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id);
void tilemapSave(Tilemap *tilemap, char *filename);
void tilemapDraw(Tilemap *tilemap);
void tilemapUnload(Tilemap *tilemap);
