#pragma once
#include <raylib.h>

void hamsterInit();
void hamsterUpdate();
void hamsterCameraUpdate();
Camera2D *hamsterGetCamera();
Vector2 hamsterGetPosition();
void hamsterDraw();
