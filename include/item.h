#pragma once
#include <raylib.h>

typedef enum _ItemType { ITEM_LINE, ITEM_NONE } ItemType;

struct _Item {
    ItemType type;
    Rectangle position_rec;
    Rectangle const *texture_rec;
    void (*on_pickup)(void);
};
typedef struct _Item Item;

void itemSpawn(Item *item, ItemType type, Vector2 position);
void itemDraw(Item *item);
