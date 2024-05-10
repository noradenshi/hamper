#include "alley.h"
#include "editor.h"
#include "gamestate.h"
#include "hamster.h"
#include "keybinds.h"
#include "levels.h"
#include "menu.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "hamper");
    SetExitKey(KEY_NULL);

    resourcesInit(); // initializes animations as well
    hamsterInit();
    levelsLoad();

    SetTargetFPS(60);
    gstateSet(GSTATE_MENU);

    while (!WindowShouldClose()) {
        if (GetScreenHeight() != window_data.HEIGHT ||
            GetScreenWidth() != window_data.WIDTH) {
            windowSizeUpdate();
            hamsterCameraUpdate();
            editorCameraUpdate();
        }

        keybindUpdate(ACTIONS_GLOBAL);

        switch (gstateGet()) {
        case GSTATE_MENU:
            // TODO;
            break;
        case GSTATE_PLAYING:
            keybindUpdate(ACTIONS_EDITOR);
            keybindUpdate(ACTIONS_GAME);
            hamsterUpdate();
            hamsterHandleCollisions(
                tilemapGetCollisions(levelsGet(), hamsterGetRect()));

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
        case GSTATE_MENU:
            menuDraw();
            break;
        case GSTATE_PLAYING:
            BeginMode2D(*hamsterGetCamera());
            tilemapDraw(levelsGet());
            hamsterDraw();
            alleyDraw();
            EndMode2D();
            break;

        case GSTATE_EDITOR:
            BeginMode2D(*editorGetCamera());
            tilemapDraw(levelsGet());
            hamsterDraw();
            alleyDraw();
            editorDrawEnd2D();
            break;
        }

        if (IS_DEBUG && gstateGet() != GSTATE_MENU) {
            DrawText(TextFormat("collisions: %d", tilemapGetCollisionsSize()),
                     5, 24, 24, WHITE);
            DrawText(
                TextFormat("jump buffer: %.4f", hamsterGetJumpBufferTime()), 5,
                24 * 2, 24, WHITE);
            DrawText(
                TextFormat("ground buffer: %.4f", hamsterGetGroundBufferTime()),
                5, 24 * 3, 24, WHITE);

            //            DrawRectangle(10, 24 * 4 + 10, 128, 8, WHITE);
            //            DrawRectangle(
            //                10, 24 * 4 + 10,
            //                128 * ((float)alleyGetPointCount() /
            //                alleyGetMaxPointCount()), 8, BLACK);
            gstateDebug(10, window_data.HEIGHT - 35, 30);
        }

        DrawFPS(0, 0);
        EndDrawing();
    }

    levelsUnload();
    resourcesUnload();
    CloseWindow();
    return 0;
}
