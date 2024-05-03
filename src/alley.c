#include "alley.h"
#include "animation.h"
#include "resources.h"
#include <raylib.h>
#include <raymath.h>

#define ALLEY_SIZE 16
#define ALLEY_CURSOR_SIZE 2

Vector2 alley_pos = {0};
Vector2 alley_target_pos = {0};
Vector2 alley_target_offset = {0};
Vector2 alley_origin = {0};
Vector2 alley_target_origin = {0};

Vector2 point_start = {0};
Vector2 point_end = {0};

Rectangle alley_target_rec =
    (Rectangle){0, 0, ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE};

void alleySetTarget(Vector2 mouse_pos) {
    if (alley_pos.x > mouse_pos.x) {
        animationSetFlipped(animations.alley_idle, true);
        alley_target_origin = (Vector2){0, ALLEY_SIZE};

        alley_target_rec.width = -ALLEY_CURSOR_SIZE;
        alley_target_offset = (Vector2){0, ALLEY_CURSOR_SIZE};
    } else {
        animationSetFlipped(animations.alley_idle, false);
        alley_target_origin = (Vector2){ALLEY_SIZE, ALLEY_SIZE};

        alley_target_rec.width = ALLEY_CURSOR_SIZE;
        alley_target_offset = (Vector2){ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE};
    }

    alley_target_pos = mouse_pos;
}

void alleyUpdate() {
    alley_origin = Vector2Lerp(alley_origin, alley_target_origin, .1f);
    alley_pos = Vector2Lerp(alley_pos, alley_target_pos, 0.1f);
    animationUpdate(animations.alley_idle);

    if (IsMouseButtonPressed(0)) {
        point_start = alley_target_pos;
    }
    if (IsMouseButtonDown(0)) {
        point_end = alley_target_pos;
    }
}

void alleyDraw() {
    DrawLineEx(point_start, point_end, 1.f, WHITE);

    DrawTexturePro(
        textures.hamster, *animationGetFrame(animations.alley_idle),
        (Rectangle){alley_pos.x, alley_pos.y, ALLEY_SIZE, ALLEY_SIZE},
        alley_origin, 0.f, WHITE);
    DrawTexturePro(textures.cursor, alley_target_rec,
                   (Rectangle){alley_target_pos.x, alley_target_pos.y,
                               ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE},
                   alley_target_offset, 0.f, WHITE);
}
