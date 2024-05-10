#pragma once
#include <raylib.h>

// more like scenes really
typedef enum _GameState {
    GSTATE_MENU,
    GSTATE_PLAYING,
    GSTATE_EDITOR,
} GameState;

extern bool IS_DEBUG;

GameState gstateGet();
void gstateSet(GameState state);
void gstateDebug(float x, float y, int fontsize);
