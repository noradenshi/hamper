#pragma once
#include "tilemap.h"
#include <raylib.h>

typedef enum _Level { LEVEL_MENU, LEVEL_TMP, LEVEL_SIZE } Level;

void levelsLoad();
Tilemap *levelsGet(Level level);
const char *levelsFilename(Level level);
void levelsUnload();
