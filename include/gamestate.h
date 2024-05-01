#pragma once
#include <raylib.h>

typedef enum _GameState {
    GSTATE_PLAYING,
    GSTATE_EDITOR,
} GameState;

GameState gstateGet();
void gstateSet(GameState state);
void gstateDebug(float x, float y, int fontsize);
