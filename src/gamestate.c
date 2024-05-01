#include "gamestate.h"

GameState gameState;

GameState gstateGet() { return gameState; }

void gstateSet(GameState state) { gameState = state; }

void gstateDebug(float x, float y, int fontsize) {
    switch (gameState) {
    case GSTATE_PLAYING:
        DrawText("GAME", x, y, fontsize, WHITE);
        break;
    case GSTATE_EDITOR:
        DrawText("EDITOR", x, y, fontsize, WHITE);
        break;
    }
}
