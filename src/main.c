#include "editor.h"
#include "hamster.h"
#include "keybind.h"
#include "gamestate.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "hamper");
    // SetExitKey(KEY_NULL);

    KeyboardKey *keybinds = keybindInit();
    resourcesInit();
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

        switch (gstateGet()) {
        case GSTATE_PLAYING:
            keybindUpdate(keybinds);
            hamsterUpdate();
            break;

        case GSTATE_EDITOR:
            keybindUpdate(keybinds);
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

        gstateDebug(10, window_data.HEIGHT - 35, 30);
        DrawFPS(0, 0);
        EndDrawing();
    }

    tilemapUnload(test_map);
    resourcesUnload();
    return 0;
}
