#include "editor.h"
#include "hamster.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

enum _GameState {
    GSTATE_PLAYING,
    GSTATE_EDITOR,
} gameState;

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
int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "hamper");

    resourcesInit();
    hamsterInit();
    Tilemap *const test_map = tilemapLoad("resources/test_tmp");
    editorInit(test_map);

    SetTargetFPS(60);
    gameState = GSTATE_PLAYING;

    while (!WindowShouldClose()) {
        if (GetScreenHeight() != window_data.HEIGHT ||
            GetScreenWidth() != window_data.WIDTH) {
            windowSizeUpdate();
            hamsterCameraUpdate();
            editorCameraUpdate();
        }

        switch (gameState) {
        case GSTATE_PLAYING:
            if (IsKeyPressed(KEY_E)) {
                gameState = GSTATE_EDITOR;
            }

            hamsterUpdate();
            break;

        case GSTATE_EDITOR:
            if (IsKeyPressed(KEY_G)) {
                gameState = GSTATE_PLAYING;
            }

            editorUpdate();
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameState) {
        case GSTATE_PLAYING:
            BeginMode2D(*hamsterGetCamera());
            tilemapDraw(test_map);
            hamsterDraw();
            EndMode2D();
            break;

        case GSTATE_EDITOR:
            BeginMode2D(*editorGetCamera());
            tilemapDraw(test_map);
            hamsterDraw();
            editorDrawEnd2D();
            break;
        }

        gstateDebug(10, window_data.HEIGHT - 35, 30);
        DrawFPS(0, 0);
        EndDrawing();
    }

    tilemapUnload(test_map);
    resourcesUnload();
    return 0;
}
