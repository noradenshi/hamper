#include "gamestate.h"
#include "alley.h"
#include "editor.h"
#include "hamster.h"
#include "levels.h"
#include <raylib.h>

GameState gameState;
bool IS_DEBUG = true;

GameState gstateGet() { return gameState; }

void gstateSet(GameState state) {
    gameState = state;
    switch (gameState) {
    case GSTATE_MENU:
        ShowCursor();
        break;
    case GSTATE_PLAYING:
        HideCursor();
        alleySetTarget(
            GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));
        break;
    case GSTATE_EDITOR:
        ShowCursor();
        editorSetTilemap(levelsGet());
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
