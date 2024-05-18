#include "menu.h"
#include "animation.h"
#include "button.h"
#include "gamestate.h"
#include "levels.h"
#include "resources.h"
#include <raylib.h>

#define MENU_BUTTON_WIDTH 220
#define MENU_BUTTON_HEIGHT 60
#define MENU_BUTTON_OFFSET 2.5f
#define MENU_PAD 4.f
#define MENU_TILE_ZOOM 8.f
#define MENU_TILE_SIZE 16 * MENU_TILE_ZOOM

#define MENU_TITLE_

const Color TRANSPARENT = (Color){0};

Camera2D menu_camera = (Camera2D){.zoom = MENU_TILE_ZOOM};

const float menu_title_cooldown = 2.f;
float menu_title_timer = menu_title_cooldown;
const char *menu_titles[] = {"hamster jumper", "HAMster jumPER", "HAMPER"};
int menu_title_id = 0;
const int menu_titles_size = 3;

int menu_buttons_size;

void menu_play() { gstateSet(GSTATE_PLAYING); }
void menu_editor() { gstateSet(GSTATE_EDITOR); }
void menu_settings() { TraceLog(LOG_INFO, "TODO"); }
void menu_exit() { gstateExit(); }

Button buttons[] = {{.text = "PLAY", &menu_play},
                    {.text = "EDITOR", &menu_editor},
                    //{.text = "SETTINGS", &menu_settings},
                    {.text = "EXIT", &menu_exit}};

void menuInit() {
    animationSetFlipped(animations.alley_idle, true);
    animationSetFlipped(animations.hamster_idle, false);
    menu_buttons_size = sizeof(buttons) / sizeof(buttons[0]);
}

Camera2D *menuGetCamera() { return &menu_camera; }

void menuUpdate() {
    for (int i = 0; i < menu_buttons_size; i++) {
        buttons[i].rectangle = (Rectangle){
            window_data.WIDTH / 2.f - MENU_BUTTON_WIDTH / 2.f,
            window_data.HEIGHT - MENU_PAD * (menu_buttons_size - i) -
                MENU_BUTTON_HEIGHT *
                    (menu_buttons_size + MENU_BUTTON_OFFSET - i),
            MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
        buttonUpdate(&buttons[i]);
    }
    animationUpdate(animations.hamster_idle);
    animationUpdate(animations.alley_idle);

    menu_title_timer -= GetFrameTime();
    if (menu_title_timer < 0.f) {
        menu_title_timer = menu_title_cooldown;
        menu_title_id++;
        if (menu_title_id >= menu_titles_size)
            menu_title_id = 0;
    }

    // idk
    menu_camera.offset.x = window_data.WIDTH / 2.f;
    menu_camera.offset.y = window_data.HEIGHT / 2.f;
}

void menuDraw() {
    BeginMode2D(menu_camera);
    tilemapDraw(levelsGet(LEVEL_MENU));
    DrawTexturePro(textures.players,
                   *animationGetFrame(animations.hamster_idle),
                   (Rectangle){40, -16, 16, 16}, (Vector2){0}, 0.f, WHITE);
    DrawTexturePro(textures.players, *animationGetFrame(animations.alley_idle),
                   (Rectangle){56, -20, 16, 16}, (Vector2){0}, 0.f, WHITE);
    EndMode2D();

    DrawText("by noradenshi",
             (window_data.WIDTH - MeasureText("by noradenshi", 32)) / 2.f,
             window_data.HEIGHT - 36.f, 32, GRAY);

    DrawText(menu_titles[menu_title_id],
             window_data.WIDTH / 2.f -
                 MeasureText(menu_titles[menu_title_id], 64) / 2.f,
             180, 64, WHITE);

    for (int i = 0; i < menu_buttons_size; i++)
        buttonDraw(&buttons[i]);
}
