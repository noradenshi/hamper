#pragma once
#include "tilemap.h"
#include <raylib.h>

int alleyGetPointCount();
int alleyGetMaxPointCount();
void alleySetTarget(Vector2 mouse_pos);
Rectangle alleyLineGetRect();
void alleyHandleCollisions(Collisions *collisions);
void alleyUpdate();
void alleyDraw();
