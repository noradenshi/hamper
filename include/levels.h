#pragma once
#include "tilemap.h"
#include <raylib.h>

typedef enum _Level { LEVEL_MENU, LEVEL_TMP, LEVEL_SIZE } Level;

void levelsLoad();
const char *levelsFilename(Level level);
void levelsUnload();

float levelGetGravity(Level level);
void levelHandleCollisions(Level level);
void levelUpdateEntities(Level level);
void levelDraw(Level level);
Tilemap *levelGetTilemap(Level level);
