#include "alley.h"
#include "editor.h"
#include "gamestate.h"
#include "hamster.h"
#include "keybind.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

// TODO: level manager
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
    alleySetTarget(GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));

    while (!WindowShouldClose()) {
        if (GetScreenHeight() != window_data.HEIGHT ||
            GetScreenWidth() != window_data.WIDTH) {
            windowSizeUpdate();
            hamsterCameraUpdate();
            editorCameraUpdate();
        }

        keybindUpdate(ACTIONS_GLOBAL);

        switch (gstateGet()) {
        case GSTATE_PLAYING:
            keybindUpdate(ACTIONS_EDITOR);
            keybindUpdate(ACTIONS_GAME);
            hamsterUpdate();
            hamsterHandleCollisions(
                tilemapGetCollisions(test_map, hamsterGetRect()));

            // if (GetMouseDelta().x != .0f || GetMouseDelta().y != .0f)
            alleySetTarget(
                GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));
            alleyUpdate();
            break;

        case GSTATE_EDITOR:
            keybindUpdate(ACTIONS_EDITOR);
            editorUpdate();
            break;
        }

        BeginDrawing();
        ClearBackground((Color){90, 110, 120, 255});

        switch (gstateGet()) {
        case GSTATE_PLAYING:
            BeginMode2D(*hamsterGetCamera());
            tilemapDraw(test_map);
            hamsterDraw();
            alleyDraw();
            EndMode2D();
            break;

        case GSTATE_EDITOR:
            BeginMode2D(*editorGetCamera());
            tilemapDraw(test_map);
            hamsterDraw();
            alleyDraw();
            editorDrawEnd2D();
            break;
        }

        if (IS_DEBUG) {
            DrawText(TextFormat("jump buffer: %.4f", hamsterGetJBT()), 5, 48,
                     24, WHITE);
            DrawText(TextFormat("collisions: %d", tilemapGetCollisionsSize()),
                     5, 24, 24, WHITE);
            DrawText(TextFormat("point count: %d", alleyGetPointCount()), 5,
                     24 * 3, 24, WHITE);

            DrawRectangle(10, 24 * 4 + 10, 128, 8, WHITE);
            DrawRectangle(
                10, 24 * 4 + 10,
                128 * ((float)alleyGetPointCount() / alleyGetMaxPointCount()),
                8, BLACK);
        }
        gstateDebug(10, window_data.HEIGHT - 35, 30);
        DrawFPS(0, 0);
        EndDrawing();
    }

    tilemapUnload(test_map);
    resourcesUnload();
    return 0;
}
