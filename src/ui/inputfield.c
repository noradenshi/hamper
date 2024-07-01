#include "ui/inputfield.h"
#include <raylib.h>

void inputfieldUpdate(InputField *inputfield) {
    if (inputfield->is_focused) {
        KeyboardKey key = GetKeyPressed();

        switch (key) {
        case KEY_ENTER:
            inputfield->on_submit();
        case KEY_ESCAPE:
        case KEY_TAB:
            inputfield->is_focused = false;
            break;
        case KEY_BACKSPACE:
            if (inputfield->cursor_pos > 0)
                inputfield->cursor_pos--;

            inputfield->text[inputfield->cursor_pos] = 0;
            break;
        case KEY_COMMA:
        case KEY_PERIOD:
        case KEY_SLASH:
        case KEY_SEMICOLON:
        case KEY_EQUAL:
            break;
        default:
            if (inputfield->cursor_pos >= TEXT_MAX_LENGTH)
                break;
            if ((key >= KEY_APOSTROPHE && key <= KEY_NINE) ||
                key == KEY_SPACE) {
                inputfield->text[inputfield->cursor_pos] = (char)key;
                inputfield->cursor_pos++;
                break;
            }
            if (key >= KEY_A && key <= KEY_Z) {
                inputfield->text[inputfield->cursor_pos] =
                    (IsKeyDown(KEY_LEFT_SHIFT)) ? key : (key + 32);
                inputfield->cursor_pos++;
                break;
            }
        }
    }

    inputfield->is_hover = false;

    if (CheckCollisionPointRec(GetMousePosition(), inputfield->rectangle)) {
        inputfield->is_hover = true;

        if (IsMouseButtonPressed(0))
            inputfield->is_focused = true;
    }
}

void inputfieldSubmit(InputField *inputfield) {
    inputfield->on_submit();
    inputfield->is_focused = false;
}

void inputfieldDraw(InputField *inputfield) {
    DrawRectangleRec(inputfield->rectangle,
                     (inputfield->is_hover) ? LIGHTGRAY : WHITE);

    DrawText(inputfield->text,
             inputfield->rectangle.x +
                 (inputfield->rectangle.width -
                  MeasureText(inputfield->text, inputfield->font_size)) /
                     2.f,
             inputfield->rectangle.y + 15.f, inputfield->font_size, BLACK);
}
