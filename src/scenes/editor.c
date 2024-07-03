#include "scenes/editor.h"
#include "gamestate.h"
#include "levels.h"
#include "resources.h"
#include "tilemap.h"
#include "ui/inputfield.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

struct _Selection {
    int id;
    int col;
    int row;
    Vector2 mouse_tile;

    // odd pal, prolly gon' refactor it later
    Vector2 position;
} selection;

struct _Toolbox {
    Vector2 position;
    float zoom;
    float scale; // for tile scaling
    bool is_hover;
} toolbox = {.position = {-1, -1}, .zoom = 2.f};

Camera2D editor_camera = {.zoom = 1.f};

const char levels_path[] = "resources/levels/";

#define SAVE_PROMPT_WIDTH 400
#define SAVE_PROMPT_FONTSIZE 40
void saveAsCallback(void);

bool save_as_is_visible = false;

const char save_as_text[] = "Save as:";

InputField save_as_inputfield = {
    (Rectangle){0, 0, SAVE_PROMPT_WIDTH, SAVE_PROMPT_FONTSIZE * 1.5f},
    SAVE_PROMPT_FONTSIZE, saveAsCallback};

bool mouse_is_held = false;

Camera2D *editorGetCamera() { return &editor_camera; }

void editorShowSaveAsDialog() {
    save_as_is_visible = true;
    save_as_inputfield.is_focused = true;
}

bool editorIsDialog() { return save_as_is_visible; }

void editorHideDialog() {
    save_as_is_visible = false;
    save_as_inputfield.is_focused = false;
}

void editorSave() {
    tilemapSave(levelGetTilemap(active_level));
    TraceLog(LOG_INFO, "Saved!");
}

void editorSaveAs(const char *filename) {
    char *path = malloc(strlen(levels_path) * sizeof(char) +
                        strlen(filename) * sizeof(char) + 1);
    strcpy(path, levels_path);
    strcat(path, filename);

    tilemapSetPath(levelGetTilemap(active_level), path);
    free(path);

    editorSave();
}

void saveAsCallback(void) {
    editorSaveAs(save_as_inputfield.text);
    save_as_is_visible = false;
}

void editorCameraUpdate() { editor_camera.offset = window_data.CENTER; }

void editorUpdate() {
    if (save_as_is_visible) {
        save_as_inputfield.rectangle.x =
            window_data.CENTER.x - SAVE_PROMPT_WIDTH / 2.f;
        save_as_inputfield.rectangle.y =
            window_data.CENTER.y - SAVE_PROMPT_FONTSIZE / 2.f;
        inputfieldUpdate(&save_as_inputfield);

        if (!save_as_inputfield.is_hover && save_as_inputfield.is_focused &&
            IsMouseButtonPressed(0))
            save_as_inputfield.is_focused = false;

        return;
    }

    if (IsMouseButtonDown(2)) {
        editor_camera.target = Vector2Add(
            editor_camera.target,
            Vector2Scale(GetMouseDelta(), -1.f / editor_camera.zoom));
    }

    toolbox.is_hover =
        (GetMouseX() > toolbox.position.x && GetMouseY() > toolbox.position.y);

    if (toolbox.is_hover) {
        if (GetMouseWheelMove() != 0) {
            toolbox.zoom *= (GetMouseWheelMove() > 0.f) ? 1.25f : 0.8f;
            toolbox.scale = TILE_SIZE * toolbox.zoom;
        }

        toolbox.position.x =
            window_data.WIDTH - textures.tileset.width * toolbox.zoom;
        toolbox.position.y =
            window_data.HEIGHT - textures.tileset.height * toolbox.zoom;

        selection.position =
            (Vector2){toolbox.position.x + selection.col * toolbox.scale,
                      toolbox.position.y + selection.row * toolbox.scale};

        if (IsMouseButtonPressed(0)) {
            toolbox.scale = TILE_SIZE * toolbox.zoom;
            selection.row = (GetMouseY() - toolbox.position.y) / toolbox.scale;
            selection.col = (GetMouseX() - toolbox.position.x) / toolbox.scale;
            selection.id =
                selection.row * textures.tileset.width + selection.col;
        }

        return;
    } // toolbox.is_hover

    if (GetMouseWheelMove() != 0) {
        editor_camera.zoom *= (GetMouseWheelMove() > 0.f) ? 1.25f : 0.8f;
    }

    selection.mouse_tile =
        GetScreenToWorld2D(GetMousePosition(), editor_camera);
    selection.mouse_tile.x -= (selection.mouse_tile.x < 0) ? 8 : 0;
    selection.mouse_tile.y -= (selection.mouse_tile.y < 0) ? 8 : 0;
    selection.mouse_tile.x =
        (int)selection.mouse_tile.x - (int)selection.mouse_tile.x % 8;
    selection.mouse_tile.y =
        (int)selection.mouse_tile.y - (int)selection.mouse_tile.y % 8;

    if (IsMouseButtonPressed(0))
        mouse_is_held = true;

    if (mouse_is_held && IsMouseButtonDown(0)) {
        tilemapSetTile(levelGetTilemap(active_level),
                       (Rectangle){selection.mouse_tile.x,
                                   selection.mouse_tile.y, TILE_SIZE,
                                   TILE_SIZE},
                       selection.id);
    }

    if (IsMouseButtonDown(1)) {
        tilemapSetTile(levelGetTilemap(active_level),
                       (Rectangle){selection.mouse_tile.x,
                                   selection.mouse_tile.y, TILE_SIZE,
                                   TILE_SIZE},
                       -1);
    }
}

void editorDrawEnd2D() {
    if (!toolbox.is_hover) {
        DrawTexturePro(
            textures.tileset, recs.tileset[selection.id],
            (Rectangle){selection.mouse_tile.x, selection.mouse_tile.y, 8, 8},
            (Vector2){0, 0}, 0.f, WHITE);
    }
    EndMode2D();

    DrawRectangle(toolbox.position.x, toolbox.position.y,
                  textures.tileset.width * toolbox.zoom,
                  textures.tileset.height * toolbox.zoom,
                  (Color){80, 80, 80, 200});
    DrawTextureEx(textures.tileset, toolbox.position, 0.f, toolbox.zoom, WHITE);

    DrawRectangleLinesEx((Rectangle){selection.position.x, selection.position.y,
                                     toolbox.scale, toolbox.scale},
                         1.2f, WHITE);

    if (toolbox.is_hover) {
        const int col = (GetMouseX() - toolbox.position.x) / toolbox.scale;
        const int row = (GetMouseY() - toolbox.position.y) / toolbox.scale;
        DrawRectangle(toolbox.position.x + toolbox.scale * col,
                      toolbox.position.y + toolbox.scale * row, toolbox.scale,
                      toolbox.scale, (Color){200, 200, 200, 100});
    }

    if (save_as_is_visible) {
        DrawRectangle(
            save_as_inputfield.rectangle.x - 20,
            save_as_inputfield.rectangle.y - SAVE_PROMPT_FONTSIZE - 20,
            SAVE_PROMPT_WIDTH + 40,
            save_as_inputfield.rectangle.height + SAVE_PROMPT_FONTSIZE + 40,
            (Color){120, 120, 120, 100});

        DrawText(save_as_text, save_as_inputfield.rectangle.x,
                 save_as_inputfield.rectangle.y - SAVE_PROMPT_FONTSIZE,
                 SAVE_PROMPT_FONTSIZE, BLACK);

        inputfieldDraw(&save_as_inputfield);
    }
}
