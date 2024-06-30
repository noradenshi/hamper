#pragma once
#include "tilemap.h"
#include <raylib.h>

void hamsterInit();
void hamsterReset();
void hamsterSetPosition(float x, float y);
void hamsterMove(short direction);
void hamsterJump();

float hamsterGetJumpBufferTime();
float hamsterGetGroundBufferTime();

Rectangle *hamsterGetRect();
void hamsterHandleCollisions(Collisions *collisions);
void hamsterUpdate();
void hamsterCameraUpdate();
Camera2D *hamsterGetCamera();
void hamsterDraw();
void hamsterDrawHUD();
