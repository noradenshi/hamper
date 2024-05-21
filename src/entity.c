#include "entity.h"
#include "alley.h"
#include "resources.h"
#include <raylib.h>

#define ENTITY_BOX_SIZE 8
const Rectangle entity_box_rec = (Rectangle){0, 0, 8, 8};

void entity_line_pickup() { alleyAddLines(1); }

void entitySpawn(Entity *entity, EntityType type, Vector2 position) {
    entity->type = type;
    entity->position_rec =
        (Rectangle){position.x, position.y, ENTITY_BOX_SIZE, ENTITY_BOX_SIZE};
    entity->texture_rec = &entity_box_rec;

    switch (type) {
    case ENTITY_BOX:
        entity->on_pickup = entity_line_pickup;
        break;
    default:
        break;
    }
}

void entityDraw(Entity *entity) {
    DrawTexturePro(textures.entities, *entity->texture_rec,
                   entity->position_rec, (Vector2){0}, 0.f, WHITE);
}
