#pragma once
#include <raylib.h>

typedef struct _Tile {
    Rectangle pos;
    int src_id;
} Tile;

typedef struct _Tilemap {
    int capacity;
    int size;
    Tile *tiles;
} Tilemap;

Tilemap *tilemapEmpty();
Tilemap *tilemapLoad(char *filename);
void tilemapSetTile(Tilemap *tilemap, Rectangle pos, int src_id);
void tilemapSave(Tilemap *tilemap, char *filename);
void tilemapDraw(Tilemap *tilemap);
void tilemapUnload(Tilemap *tilemap);
