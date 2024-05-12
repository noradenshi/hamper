#pragma once
#include "levels.h"
#include <raylib.h>

// more like scenes really
typedef enum _GameState {
    GSTATE_MENU,
    GSTATE_PLAYING,
    GSTATE_EDITOR,
} GameState;

extern bool IS_DEBUG;
extern Level active_level;

GameState gstateGet();
void gstateSet(GameState state);
void gstateDebug(float x, float y, int fontsize);
void gstateExit();
