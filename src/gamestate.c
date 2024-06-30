#include "gamestate.h"
#include "entities/alley.h"
#include "scenes/editor.h"
#include "entities/hamster.h"
#include "levels.h"
#include "scenes/menu.h"
#include "resources.h"
#include <raylib.h>
#include <stdlib.h>

GameState gameState;
bool IS_DEBUG = false;
Level active_level = LEVEL_MENU;

GameState gstateGet() { return gameState; }

void gstateSet(GameState state) {
    gameState = state;
    switch (gameState) {
    case GSTATE_MENU:
        menuInit();
        ShowCursor();
        break;
    case GSTATE_PLAYING:
        active_level = LEVEL_TMP;
        HideCursor();
        alleySetTarget(
            GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));
        break;
    case GSTATE_EDITOR:
        ShowCursor();
        editorSetTilemap(levelGetTilemap(active_level));
        break;
    }
}

void gstateDebug(float x, float y, int fontsize) {
    switch (gameState) {
    case GSTATE_PLAYING:
        DrawText("GAME", x, y, fontsize, WHITE);
        break;
    case GSTATE_EDITOR:
        DrawText("EDITOR", x, y, fontsize, WHITE);
        break;
    default:
        break;
    }
}

void gstateExit() {
    levelsUnload();
    resourcesUnload();
    CloseWindow();
    exit(0);
}
