#include "entity.h"
#include "resources.h"
#include <raylib.h>

#define ENTITY_TILE_SIZE 8
const Rectangle entity_box_rec = (Rectangle){0, 0, 8, 8};

void entitySpawn(Entity *entity, EntityType type, Vector2 position) {
    entity->type = type;
    entity->position_rec =
        (Rectangle){position.x, position.y, ENTITY_TILE_SIZE, ENTITY_TILE_SIZE};

    switch (type) {
    case ENTITY_BOX:
        entity->texture_rec = &entity_box_rec;
        entity->is_stationary = true;
        break;
    default:
        break;
    }
}

void entityDraw(Entity *entity) {
    DrawTexturePro(textures.entities, *entity->texture_rec,
                   entity->position_rec, (Vector2){0}, 0.f, WHITE);
}
