#pragma once
#include <raylib.h>

Camera2D *editorGetCamera();
void editorCameraUpdate();
void editorUpdate();
void editorSave();
void editorShowSaveAsDialog();
bool editorIsDialog();
void editorHideDialog();
void editorDrawEnd2D();
