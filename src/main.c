#include "editor.h"
#include "gamestate.h"
#include "hamster.h"
#include "keybind.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "hamper");
    // SetExitKey(KEY_NULL);

    resourcesInit(); // initializes animations as well
    hamsterInit();
    Tilemap *const test_map = tilemapLoad("resources/test_tmp");
    editorInit(test_map);

    SetTargetFPS(60);
    gstateSet(GSTATE_PLAYING);

    while (!WindowShouldClose()) {
        if (GetScreenHeight() != window_data.HEIGHT ||
            GetScreenWidth() != window_data.WIDTH) {
            windowSizeUpdate();
            hamsterCameraUpdate();
            editorCameraUpdate();
        }

        // TODO: switch order of hamsterUpdate and hamsterHandleCollisions
        // to hopefully fix collision response stuff
        switch (gstateGet()) {
        case GSTATE_PLAYING:
            keybindUpdate(ACTIONS_EDITOR);
            keybindUpdate(ACTIONS_GAME);
            hamsterUpdate();
            hamsterHandleCollisions(
                tilemapGetCollisions(test_map, hamsterGetRect()));
            break;

        case GSTATE_EDITOR:
            keybindUpdate(ACTIONS_EDITOR);
            editorUpdate();
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (gstateGet()) {
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

        // DEBUG
        DrawText(TextFormat("collisions: %d", tilemapGetCollisionsSize()), 0,
                 24, 24, WHITE);
        gstateDebug(10, window_data.HEIGHT - 35, 30);
        DrawFPS(0, 0);
        EndDrawing();
    }

    tilemapUnload(test_map);
    resourcesUnload();
    return 0;
}
