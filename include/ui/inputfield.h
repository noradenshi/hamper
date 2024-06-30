#pragma once
#include <raylib.h>

#define TEXT_MAX_LENGTH 64

typedef struct _InputField {
    Rectangle rectangle;
    void (*on_submit)(void);
    char text[TEXT_MAX_LENGTH];
    int cursor_pos;
    bool is_hover;
    bool is_focused;
} InputField;

void inputfieldUpdate(InputField *inputfield);
void inputfieldSubmit(InputField *inputfield);
void inputfieldDraw(InputField *inputfield);
