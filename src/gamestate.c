#include "gamestate.h"
#include "entities/alley.h"
#include "entities/hamster.h"
#include "levels.h"
#include "resources.h"
#include "scenes/menu.h"
#include <raylib.h>
#include <stdlib.h>

bool IS_DEBUG = false;
Scene scene;
LevelData *active_level;

Scene sceneGet() { return scene; }

void sceneLoadLevel(const char *filename) {
    if (active_level != nullptr) {
        free(active_level);
    }

    active_level = levelLoad(filename);
}

void sceneSet(Scene _scene) {
    scene = _scene;
    switch (scene) {
    case SCENE_MENU:
        sceneLoadLevel(levelCorePath(LEVEL_MENU));
        menuInit();
        ShowCursor();
        break;
    case SCENE_PLAYING:
        sceneLoadLevel(levelCorePath(LEVEL_TMP));
        HideCursor();
        alleySetTarget(
            GetScreenToWorld2D(GetMousePosition(), *hamsterGetCamera()));
        break;
    case SCENE_EDITOR:
        ShowCursor();
        break;
    }
}

void sceneDebug(float x, float y, int fontsize) {
    switch (scene) {
    case SCENE_PLAYING:
        DrawText("GAME", x, y, fontsize, WHITE);
        break;
    case SCENE_EDITOR:
        DrawText("EDITOR", x, y, fontsize, WHITE);
        break;
    default:
        break;
    }
}

void sceneExit() {
    levelUnload(active_level);
    resourcesUnload();
    CloseWindow();
    exit(0);
}
