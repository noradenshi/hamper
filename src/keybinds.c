#include "keybinds.h"
#include "editor.h"
#include "gamestate.h"
#include "hamster.h"
#include "levels.h"
#include "resources.h"
#include <raylib.h>
#include <stdlib.h>

typedef enum _GlobalAction {
    GLOBAL_DEBUG,
    GLOBAL_EXIT,
    GLOBAL_END
} GlobalAction;

typedef enum _EditorAction {
    EDITOR_PLAYING,
    EDITOR_EDITOR,
    EDITOR_SAVE,
    EDITOR_END
} EditorAction;

typedef enum _GameAction {
    GAME_MOVE_LEFT,
    GAME_MOVE_RIGHT,
    GAME_JUMP,
    GAME_RESET,
    GAME_END
} GameAction;

KeyboardKey global[GLOBAL_END] = {KEY_F3, KEY_Q};
KeyboardKey editor[EDITOR_END] = {KEY_G, KEY_E, KEY_S};
// KeyboardKey game[GAME_END] = {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_R};
KeyboardKey game[GAME_END] = {KEY_A, KEY_D, KEY_W, KEY_R};

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
                levelsUnload();
                resourcesUnload();
                CloseWindow();
                exit(0);
                break;
            }
        }
        break;

    case ACTIONS_EDITOR:
        for (int i = 0; i < EDITOR_END; i++) {
            if (!IsKeyPressed(editor[i]))
                continue;
            switch (i) {

            case EDITOR_PLAYING:
                gstateSet(GSTATE_PLAYING);
                break;

            case EDITOR_EDITOR:
                gstateSet(GSTATE_EDITOR);
                break;

            case EDITOR_SAVE:
                editorSave();
                break;
            }
        }
        break;

        // gaem do gaem
    case ACTIONS_GAME:
        hamsterMove(-IsKeyDown(game[GAME_MOVE_LEFT]) +
                    IsKeyDown(game[GAME_MOVE_RIGHT]));

        if (IsKeyPressed(game[GAME_JUMP]) ||
            IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            hamsterJump();

        if (IsKeyPressed(game[GAME_RESET])) {
            hamsterReset();
        }
    }
}
