#pragma once
#include <raylib.h>
#include "tilemap.h"

void editorSetTilemap(Tilemap *tilemap);
Camera2D *editorGetCamera();
void editorCameraUpdate();
void editorUpdate();
void editorSave();
void editorDrawEnd2D();
