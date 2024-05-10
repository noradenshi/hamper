#include "levels.h"

Tilemap *tilemaps[1];

void levelsLoad() { tilemaps[0] = tilemapLoad("resources/test_tmp"); }

Tilemap *levelsGet(){
    return tilemaps[0];
}

void levelsUnload() { tilemapUnload(tilemaps[0]); }
