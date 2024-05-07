#pragma once
#include <raylib.h>

int alleyGetPointCount();
int alleyGetMaxPointCount();
void alleySetTarget(Vector2 mouse_pos);
void alleyUpdate();
void alleyDraw();

bool alleyLineCheckCollisions(Vector2 *point);
Vector2 alleyLineGetNormal();
