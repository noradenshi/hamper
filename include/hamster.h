#pragma once
#include "tilemap.h"
#include <raylib.h>

void hamsterInit();
void hamsterMove(short direction);
Rectangle hamsterGetRect();
void hamsterHandleCollisions(Collisions *collisions);
void hamsterJump();
void hamsterUpdate();
void hamsterCameraUpdate();
Camera2D *hamsterGetCamera();
void hamsterDraw();
