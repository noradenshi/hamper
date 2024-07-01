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
Tilemap *active_tilemap;

#define SAVE_PROMPT_WIDTH 400
#define SAVE_PROMPT_FONTSIZE 40
void saveAsCallback(void);

InputField file_save_as = {
    (Rectangle){0, 0, SAVE_PROMPT_WIDTH, SAVE_PROMPT_FONTSIZE * 1.5f},
    SAVE_PROMPT_FONTSIZE, saveAsCallback};

bool mouse_is_held = false;

Camera2D *editorGetCamera() { return &editor_camera; }

const char levels_path[] = "resources/levels/";
void editorSaveAs(const char *filename) {
    char *path = malloc(strlen(levels_path) + strlen(filename));
    strcpy(path, levels_path);
    tilemapSave(active_tilemap, strcat(path, filename));
    TraceLog(LOG_INFO, "Saved!");
}

void saveAsCallback(void) { editorSaveAs(file_save_as.text); }

void editorSave() { editorSaveAs(levelsFilename(active_level)); }

void editorSetTilemap(Tilemap *tilemap) { active_tilemap = tilemap; }

void editorCameraUpdate() { editor_camera.offset = window_data.CENTER; }

void editorUpdate() {
    if (true) {
        inputfieldUpdate(&file_save_as);
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
        tilemapSetTile(active_tilemap,
                       (Rectangle){selection.mouse_tile.x,
                                   selection.mouse_tile.y, TILE_SIZE,
                                   TILE_SIZE},
                       selection.id);
    }

    if (IsMouseButtonDown(1)) {
        tilemapSetTile(active_tilemap,
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

    if (true) {
        inputfieldDraw(&file_save_as);
    }
}
