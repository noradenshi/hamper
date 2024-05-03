#include "hamster.h"
#include "animation.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>

#define HAMSTER_SIZE 16

const Vector2 hamster_start_pos = {40, -36}; // center
Vector2 hamster_pos = hamster_start_pos;
Vector2 hamster_velocity = {0};

short hamster_direction = 0;
float hamster_speed = 25.f;
float hamster_jump_force = 4.f;
float hamster_gravity = 8.f;

bool hamster_is_flipped = false;
bool hamster_is_grounded = false;

// TEMP
float air_friction = 1.1f;
float grass_friction = 1.2f;

Animation *hamster_anim;
Camera2D hamster_camera;

void hamsterInit() {
    hamster_anim = animations.hamster_idle;

    hamster_camera =
        (Camera2D){.zoom = 4.21f, .target = hamster_pos, .rotation = 0.f};
    hamsterCameraUpdate(); // to apply correct offset
}

void hamsterReset() {
    hamster_pos = hamster_start_pos;
    hamster_velocity = (Vector2){0.f, 0.f};
}

void hamsterSetPosition(float x, float y) {
    hamster_pos.x = x;
    hamster_pos.y = y;
}

Rectangle hamsterGetRect() {
    return (Rectangle){hamster_pos.x, hamster_pos.y, HAMSTER_SIZE,
                       HAMSTER_SIZE + 1};
}

void hamsterHandleCollisions(Collisions *collisions) {
    hamster_is_grounded = false;
    for (int i = 0; i < collisions->size; i++) {
        if (collisions->rec[i].width > collisions->rec[i].height) {
            if (collisions->rec[i].y == hamster_pos.y) {
                hamster_velocity.y = .0f;
                hamster_pos.y += collisions->rec[i].height;
                continue;
            }
            hamster_is_grounded = true;
            hamster_velocity.y = .0f;
            hamster_pos.y = collisions->rec[i].y - HAMSTER_SIZE;
        } else {
            if (collisions->rec[i].x == hamster_pos.x) {
                hamster_velocity.x = .0f;
                hamster_pos.x += collisions->rec[i].width;
                continue;
            }
            hamster_velocity.x = .0f;
            hamster_pos.x = collisions->rec[i].x - HAMSTER_SIZE;
            continue;
        }
    }
    hamsterCameraUpdate();
}

void hamsterMove(short direction) {
    hamster_direction = direction;
    if (direction != 0) {
        hamster_is_flipped = (direction == -1);
        animationSetFlipped(hamster_anim, hamster_is_flipped);
    }

    hamster_velocity.x += hamster_direction * hamster_speed *
                          (IsKeyDown(KEY_LEFT_SHIFT) + 1) * GetFrameTime();
}

void hamsterJump() {
    if (hamster_is_grounded) {
        hamster_is_grounded = false;
        hamster_velocity.y = -hamster_jump_force;
    }
}

// do the movement, then physics
void hamsterUpdate() {
    if (hamster_direction == 0) {
        hamster_anim = animations.hamster_idle;
    } else if (hamster_anim != animations.hamster_walk) {
        hamster_anim = animations.hamster_walk;
        animationReset(hamster_anim);
    }
    animationUpdate(hamster_anim);

    hamster_pos = Vector2Add(hamster_pos, hamster_velocity);
    hamster_camera.target = Vector2Lerp(hamster_camera.target, hamster_pos, .35f);

    if (hamster_is_grounded) {
        hamster_velocity.x = hamster_velocity.x / grass_friction;
    } else {
        hamster_velocity.y += hamster_gravity * GetFrameTime();
        hamster_velocity.x = hamster_velocity.x / air_friction;
    }
}

void hamsterCameraUpdate() {
    hamster_camera.offset.x = window_data.CENTER.x - 8 * hamster_camera.zoom;
    hamster_camera.offset.y = window_data.CENTER.y;
}

Camera2D *hamsterGetCamera() { return &hamster_camera; }

void hamsterDraw() {
    DrawTextureRec(textures.hamster, *animationGetFrame(hamster_anim),
                   hamster_pos, WHITE);

    if (hamster_is_grounded)
        DrawCircle(hamster_pos.x + 8.f, hamster_pos.y + 20.f, 1, GREEN);
}
