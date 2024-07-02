#pragma once
#include "tilemap.h"
#include <raylib.h>

typedef enum _Level { LEVEL_MENU, LEVEL_TMP, LEVEL_SIZE } Level;
typedef struct _LevelData LevelData;
extern LevelData level;

LevelData *levelLoad(const char *filename);
const char *levelCorePath(Level level);
void levelUnload(LevelData *level);

float levelGetGravity(LevelData *level);
void levelHandleCollisions(LevelData *level);
void levelUpdateEntities(LevelData *level);
void levelDraw(LevelData *level);
Tilemap *levelGetTilemap(LevelData *level);
