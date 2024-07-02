#pragma once
#include "levels.h"
#include <raylib.h>

typedef enum _Scene {
    SCENE_MENU,
    SCENE_PLAYING,
    SCENE_EDITOR,
} Scene;

extern bool IS_DEBUG;
// extern Level active_level;
extern LevelData *active_level;

Scene sceneGet();
void sceneSet(Scene scene);
void sceneDebug(float x, float y, int fontsize);
void sceneExit();

void sceneLoadLevel(const char* filename);
