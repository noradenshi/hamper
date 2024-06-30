#pragma once
#include <raylib.h>

typedef struct _Button {
    Rectangle rectangle;
    const char *text;
    void (*on_click)(void);
    bool is_hover;
} Button;

void buttonUpdate(Button *button);
void buttonDraw(Button *button);
