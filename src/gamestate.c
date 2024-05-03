#include "gamestate.h"

GameState gameState;
bool IS_DEBUG = true;

GameState gstateGet() { return gameState; }

void gstateSet(GameState state) { gameState = state; }

void gstateDebug(float x, float y, int fontsize) {
    switch (gameState) {
    case GSTATE_PLAYING:
        HideCursor();
        DrawText("GAME", x, y, fontsize, WHITE);
        break;
    case GSTATE_EDITOR:
        ShowCursor();
        DrawText("EDITOR", x, y, fontsize, WHITE);
        break;
    }
}
