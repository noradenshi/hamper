#include "gamestate.h"
#include "alley.h"
#include "editor.h"
#include "hamster.h"
#include "levels.h"
#include "menu.h"
#include "resources.h"
#include <raylib.h>
#include <stdlib.h>

GameState gameState;
bool IS_DEBUG = true;
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
        HideCursor();
        alleySetTarget(
            GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));
        break;
    case GSTATE_EDITOR:
        ShowCursor();
        editorSetTilemap(levelsGet(active_level));
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
