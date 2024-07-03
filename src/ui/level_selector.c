#include "ui/level_selector.h"
#include "gamestate.h"
#include "resources.h"
#include "ui/button.h"
#include <dirent.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Button *level_buttons = {0};
int level_buttons_size = 0;
const char *level_filename_helper;
Scene target_scene;

void levelSelectorCallback() {
    char *path = malloc(strlen(levels_path) * sizeof(char) +
                        strlen(level_filename_helper) * sizeof(char) + 1);
    strcpy(path, levels_path);
    strcat(path, level_filename_helper);

    sceneLoadLevel(path);
    free(path);

    sceneSet(target_scene);
}

void levelSelectorSetContext(Scene scene) { target_scene = scene; }

void levelSelectorFetchData() {
    if (level_buttons != nullptr) {
        for (int i = 0; i < level_buttons_size; i++)
            free(level_buttons[i].text);
        free(level_buttons);
    }

    DIR *levels_dir = opendir("resources/levels/");

    if (levels_dir == NULL) {
        printf("Could not open /resources/levels directory");
        abort();
    }

    struct dirent *de;

    int amount = 0;
    while ((de = readdir(levels_dir)) != NULL) {
        if (de->d_type != 8)
            continue;

        amount++;
    }

    level_buttons = malloc(amount * sizeof(Button));
    level_buttons_size = amount;

    int size = 0;
    rewinddir(levels_dir);
    while ((de = readdir(levels_dir)) != NULL) {
        if (de->d_type != 8)
            continue;

        level_buttons[size].text = strdup(de->d_name);
        level_buttons[size].on_click = levelSelectorCallback;
        size++;
    }

    closedir(levels_dir);
    return;
}

void levelSelectorUpdate() {
    for (int i = 0; i < level_buttons_size; i++) {
        level_buttons[i].rectangle =
            (Rectangle){window_data.WIDTH / 2.f - 100,
                        window_data.HEIGHT / 2.f + 60 * i, 200, 60};

        level_filename_helper = level_buttons[i].text;
        buttonUpdate(&level_buttons[i]);
    }
}

void levelSelectorDraw() {
    for (int i = 0; i < level_buttons_size; i++) {
        buttonDraw(&level_buttons[i]);
    }
}

void levelSelectorUnload() {
    if (level_buttons != nullptr) {
        for (int i = 0; i < level_buttons_size; i++)
            free(level_buttons[i].text);
        free(level_buttons);
    }
}
