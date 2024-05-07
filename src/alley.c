#include "alley.h"
#include "animation.h"
#include "resources.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define ALLEY_SIZE 16
#define ALLEY_CURSOR_SIZE 2
#define LINE_MAX_POINTS 2
#define LINE_MAX_LENGTH 50.f

Vector2 alley_pos = {0};
Vector2 alley_target_pos = {0};
Vector2 alley_target_offset = {0};
Vector2 alley_origin = {0};
Vector2 alley_target_origin = {0};

int point_iterator = 0;
Vector2 points[LINE_MAX_POINTS];
Vector2 alley_line_normal = {0};

Rectangle alley_target_rec =
    (Rectangle){0, 0, ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE};

int alleyGetPointCount() { return point_iterator; }
int alleyGetMaxPointCount() { return LINE_MAX_POINTS; }

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
    alley_origin = Vector2Lerp(alley_origin, alley_target_origin, 0.1f);
    alley_pos = Vector2Lerp(alley_pos, alley_target_pos, 0.1f);
    animationUpdate(animations.alley_idle);

    if (IsMouseButtonPressed(0)) {
        point_iterator = 1;
        points[0] = alley_target_pos;
    }
    if (IsMouseButtonDown(0)) {
        point_iterator = 2;
        points[1] = alley_target_pos;

        if (Vector2Distance(points[0], points[1]) > LINE_MAX_LENGTH) {
            const float mouse_distance =
                (points[1].y - points[0].y) / (points[1].x - points[0].x);
            points[1].x =
                sqrtf(1 / (pow(mouse_distance, 2) + 1)) * LINE_MAX_LENGTH;
            points[1].x *= (alley_target_pos.x > points[0].x) ? 1 : -1;
            points[1].x += points[0].x;
            points[1].y =
                mouse_distance * (points[1].x - points[0].x) + points[0].y;
        }
    }
}

void alleyDraw() {
    if (point_iterator != 0) {
        //        DrawCircleV(points[0], 1.2f, WHITE);
        //        DrawCircleV(points[1], 1.2f, WHITE);
        DrawSplineLinear(points, point_iterator, 1.f, WHITE);
        DrawLineV(points[0], points[1], YELLOW);
    }

    DrawTexturePro(
        textures.players, *animationGetFrame(animations.alley_idle),
        (Rectangle){alley_pos.x, alley_pos.y, ALLEY_SIZE, ALLEY_SIZE},
        alley_origin, 0.f, WHITE);
    DrawTexturePro(textures.cursor, alley_target_rec,
                   (Rectangle){alley_target_pos.x, alley_target_pos.y,
                               ALLEY_CURSOR_SIZE, ALLEY_CURSOR_SIZE},
                   alley_target_offset, 0.f, WHITE);
}

Vector2 alleyLineGetNormal() {
    if (points[0].x > points[1].x) {
        alley_line_normal.x = -(points[1].y - points[0].y);
        alley_line_normal.y = (points[1].x - points[0].x);
    } else {
        alley_line_normal.x = (points[1].y - points[0].y);
        alley_line_normal.y = -(points[1].x - points[0].x);
    }
    return Vector2Normalize(alley_line_normal);
}

bool alleyLineCheckCollisions(Vector2 *point) {
    if (point_iterator < 2)
        return false;
    return CheckCollisionPointLine(*point, points[0], points[1], 4);
}
