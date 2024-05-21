#pragma once
#include <raylib.h>

typedef enum _EntityType { ENTITY_BOX, ENTITY_NONE } EntityType;

struct _Entity {
    EntityType type;
    Rectangle position_rec;
    Rectangle const *texture_rec;
    void (*on_pickup)(void);
};
typedef struct _Entity Entity;

void entitySpawn(Entity *entity, EntityType type, Vector2 position);
void entityDraw(Entity *entity);
