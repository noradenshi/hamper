#include "alley.h"
#include "animation.h"
#include "resources.h"
#include <raylib.h>
#include <raymath.h>

#define ALLEY_SIZE 16
#define ALLEY_CURSOR_SIZE 2
#define MAX_POINTS 64

Vector2 alley_pos = {0};
Vector2 alley_target_pos = {0};
Vector2 alley_target_offset = {0};
Vector2 alley_origin = {0};
Vector2 alley_target_origin = {0};

int point_iterator = 0;
Vector2 points[MAX_POINTS];
bool alley_is_line_initialized = false;

struct _AlleyLine {
    float *x;
    float *y;
    float *width;
    float *height;
} alleyline;

Rectangle alley_target_rec =
    (Rectangle){0, 0, ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE};

int alleyGetPointCount() { return point_iterator; }
int alleyGetMaxPointCount() { return MAX_POINTS; }

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

float absf(float x) { return (x < 0) ? -x : x; }

Rectangle alleyLineGetRect() {
    if (alley_is_line_initialized)
        return (Rectangle){*alleyline.x - 15.f, *alleyline.y - 15.f,
                           absf(*alleyline.width - *alleyline.x) + 30.f,
                           absf(*alleyline.height - *alleyline.y) + 30.f};
    return (Rectangle){0};
}

void alleyHandleCollisions(Collisions *collisions) {
    for (int i = 0; i < point_iterator; i++) {
        bool is_falling = true;
        for (int j = 0; j < collisions->size; j++) {
            if (CheckCollisionPointRec(points[i], collisions->rec[j]))
                is_falling = false;
        }
        if (is_falling) {
            points[i].y += 32.f * GetFrameTime();
        }
    }
}

void alleyUpdate() {
    alley_origin = Vector2Lerp(alley_origin, alley_target_origin, .1f);
    alley_pos = Vector2Lerp(alley_pos, alley_target_pos, 0.1f);
    animationUpdate(animations.alley_idle);

    if (IsMouseButtonPressed(0)) {
        point_iterator = 0;
        alleyline.x = &points[0].x;
        alleyline.y = &points[0].y;
        alleyline.width = &points[0].x;
        alleyline.height = &points[0].y;
        alley_is_line_initialized = true;
    }
    if (IsMouseButtonDown(0) && point_iterator != MAX_POINTS) {
        if (point_iterator == 0 || Vector2Distance(points[point_iterator - 1],
                                                   alley_target_pos) > 2.f) {
            points[point_iterator] = alley_target_pos;

            if (alley_target_pos.x < *alleyline.x)
                alleyline.x = &points[point_iterator].x;
            else if (alley_target_pos.x > *alleyline.width)
                alleyline.width = &points[point_iterator].x;

            if (alley_target_pos.y < *alleyline.y)
                alleyline.y = &points[point_iterator].y;
            else if (alley_target_pos.y > *alleyline.height)
                alleyline.height = &points[point_iterator].y;

            point_iterator++;
        }
    }
}

void alleyDraw() {
    DrawRectangleLinesEx(alleyLineGetRect(), 1.f, BLUE);
    DrawCircleV(points[0], 1.2f, WHITE);
    DrawCircleV(points[point_iterator - 1], 1.2f, WHITE);
    DrawSplineCatmullRom(points, point_iterator, 1.f, WHITE);

    DrawTexturePro(
        textures.players, *animationGetFrame(animations.alley_idle),
        (Rectangle){alley_pos.x, alley_pos.y, ALLEY_SIZE, ALLEY_SIZE},
        alley_origin, 0.f, WHITE);
    DrawTexturePro(textures.cursor, alley_target_rec,
                   (Rectangle){alley_target_pos.x, alley_target_pos.y,
                               ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE},
                   alley_target_offset, 0.f, WHITE);
}
