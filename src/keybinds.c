#include "keybinds.h"
#include "entities/hamster.h"
#include "gamestate.h"
#include "levels.h"
#include "scenes/editor.h"
#include <raylib.h>

typedef enum _GlobalAction {
    GLOBAL_DEBUG,
    GLOBAL_EXIT,
    GLOBAL_MAP_MENU,
    GLOBAL_MAP_TMP,
    GLOBAL_END
} GlobalAction;

typedef enum _EditorAction {
    EDITOR_PLAYING,
    EDITOR_SAVE,
    EDITOR_DIALOG,
    EDITOR_END
} EditorAction;

typedef enum _GameAction {
    GAME_MOVE_LEFT,
    GAME_MOVE_RIGHT,
    GAME_JUMP,
    GAME_RESET,
    GAME_DIALOG,
    GAME_EDITOR,
    GAME_END
} GameAction;

KeyboardKey global[GLOBAL_END] = {KEY_F3, KEY_Q, KEY_ONE, KEY_TWO};
KeyboardKey editor[EDITOR_END] = {KEY_G, KEY_S, KEY_ESCAPE};
// KeyboardKey game[GAME_END] = {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_R};
KeyboardKey game[GAME_END] = {KEY_A, KEY_D, KEY_W, KEY_R, KEY_ESCAPE, KEY_E};

void keybindUpdate(ActionSet set) {
    switch (set) {
    case ACTIONS_GLOBAL:
        for (int i = 0; i < GLOBAL_END; i++) {
            if (!IsKeyPressed(global[i]))
                continue;
            switch (i) {

            case GLOBAL_DEBUG:
                IS_DEBUG = !IS_DEBUG;
                break;

            case GLOBAL_EXIT:
                if (!IsKeyDown(KEY_LEFT_CONTROL))
                    break;
                sceneExit();
                break;

                // DEBUG; to be deleted
                // case GLOBAL_MAP_MENU:
                //     sceneLoadLevel(levelCorePath(LEVEL_MENU));
                //     break;
                // case GLOBAL_MAP_TMP:
                //     sceneLoadLevel(levelCorePath(LEVEL_TMP));
                //     break;
            }
        }
        break;

    case ACTIONS_EDITOR:
        for (int i = 0; i < EDITOR_END; i++) {
            if (!IsKeyPressed(editor[i]))
                continue;
            switch (i) {

            case EDITOR_PLAYING:
                sceneSet(SCENE_PLAYING);
                // TEMP
                editorSave();
                break;

            case EDITOR_SAVE:
                if (!IsKeyDown(KEY_LEFT_CONTROL))
                    break;

                if (IsKeyDown(KEY_LEFT_SHIFT)) {
                    editorShowSaveAsDialog();
                    break;
                }

                editorSave();
                break;

            case EDITOR_DIALOG:
                if (editorIsDialog()) {
                    editorHideDialog();
                    break;
                }

                sceneSet(SCENE_MENU);
                break;
            }
        }
        break;

    case ACTIONS_GAME:
        hamsterMove(-IsKeyDown(game[GAME_MOVE_LEFT]) +
                    IsKeyDown(game[GAME_MOVE_RIGHT]));

        for (int i = 2; i < GAME_END; i++) {
            if (!IsKeyPressed(game[i]))
                continue;

            switch (i) {
            case GAME_JUMP:
                hamsterJump();
                break;

            case GAME_RESET:
                hamsterReset();
                break;

            case GAME_DIALOG:
                sceneSet(SCENE_MENU);
                break;

            case GAME_EDITOR:
                sceneSet(SCENE_EDITOR);
                break;
            }
        }
    }
}
