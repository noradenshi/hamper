#pragma once
#include <raylib.h>

void hamsterInit();
void hamsterMove(bool is_left);
void hamsterJump();
void hamsterUpdate();
void hamsterCameraUpdate();
Camera2D *hamsterGetCamera();
Vector2 hamsterGetPosition();
void hamsterDraw();
