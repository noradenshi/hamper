#pragma once
#include <raylib.h>

typedef enum _ActionSet {
    ACTIONS_EDITOR,
    ACTIONS_GAME,
} ActionSet;

void keybindUpdate(ActionSet set);
