#pragma once
#include <raylib.h>

void alleySetTarget(Vector2 mouse_pos);
void alleyUpdate();
void alleyDraw();
void alleyHUDDraw();

void alleyAddLines(int amount);
bool alleyLineCheckCollisions(Vector2 *point);
Vector2 alleyLineGetNormal();
