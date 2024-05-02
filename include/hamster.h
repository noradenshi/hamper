#pragma once
#include "tilemap.h"
#include <raylib.h>

void hamsterInit();
void hamsterReset();
void hamsterSetPosition(float x, float y);
void hamsterMove(short direction);
Rectangle hamsterGetRect();
void hamsterHandleCollisions(Collisions *collisions);
void hamsterJump();
void hamsterUpdate();
void hamsterCameraUpdate();
Camera2D *hamsterGetCamera();
void hamsterDraw();
