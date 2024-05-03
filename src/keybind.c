#include "keybind.h"
#include "gamestate.h"
#include "hamster.h"
#include <raylib.h>

typedef enum _GlobalAction {
    GLOBAL_DEBUG,
    GLOBAL_EXIT,
    GLOBAL_END
} GlobalAction;

typedef enum _EditorAction {
    EDITOR_PLAYING,
    EDITOR_EDITOR,
    EDITOR_END
} EditorAction;

typedef enum _GameAction {
    GAME_MOVE_LEFT,
    GAME_MOVE_RIGHT,
    GAME_JUMP,
    GAME_RESET,
    GAME_END
} GameAction;

KeyboardKey global[GLOBAL_END] = {KEY_F3, KEY_ESCAPE};
KeyboardKey editor[EDITOR_END] = {KEY_G, KEY_E};
KeyboardKey game[GAME_END] = {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_R};

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
                // TODO
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
            }
        }
        break;

        // gaem do gaem
    case ACTIONS_GAME:
        hamsterMove(-IsKeyDown(game[GAME_MOVE_LEFT]) +
                    IsKeyDown(game[GAME_MOVE_RIGHT]));

        if (IsKeyPressed(game[GAME_JUMP]))
            hamsterJump();
        if (IsKeyPressed(game[GAME_RESET])) {
            hamsterReset();
        }
    }
}
