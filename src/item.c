#include "item.h"
#include "alley.h"
#include "resources.h"
#include <raylib.h>

#define ITEM_LINE_SIZE 8
const Rectangle item_line_rec = (Rectangle){0, 0, 8, 8};

void item_line_pickup() { alleyAddLines(1); }

void itemSpawn(Item *item, ItemType type, Vector2 position) {
    item->type = type;
    item->position_rec =
        (Rectangle){position.x, position.y, ITEM_LINE_SIZE, ITEM_LINE_SIZE};
    item->texture_rec = &item_line_rec;

    switch (type) {
    case ITEM_LINE:
        item->on_pickup = item_line_pickup;
        break;
    default:
        break;
    }
}

void itemDraw(Item *item) {
    DrawTexturePro(textures.items, *item->texture_rec, item->position_rec,
                   (Vector2){0}, 0.f, WHITE);
}
