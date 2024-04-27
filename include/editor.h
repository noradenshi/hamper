#pragma once
#include <raylib.h>
#include "tilemap.h"

void editorInit(Tilemap *tilemap);
Camera2D *editorGetCamera();
void editorCameraUpdate();
void editorUpdate();
void editorDrawEnd2D();
