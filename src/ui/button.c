#include "ui/button.h"
#include <raylib.h>

void buttonUpdate(Button *button) {
    button->is_hover = false;

    if (CheckCollisionPointRec(GetMousePosition(), button->rectangle)) {
        button->is_hover = true;

        if (IsMouseButtonPressed(0))
            button->on_click();
    }
}

void buttonDraw(Button *button) {
    DrawText(button->text,
             button->rectangle.x +
                 (button->rectangle.width - MeasureText(button->text, 36.f)) /
                     2.f,
             button->rectangle.y + 15.f, 36.f,
             (button->is_hover) ? LIGHTGRAY : WHITE);

    if (button->is_hover) {
        DrawCircle(button->rectangle.x,
                   button->rectangle.y + button->rectangle.height / 2.f, 6.f,
                   LIGHTGRAY);
        DrawCircle(button->rectangle.x + button->rectangle.width,
                   button->rectangle.y + button->rectangle.height / 2.f, 6.f,
                   LIGHTGRAY);
    }
}
