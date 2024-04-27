#include "hamster.h"
#include "animation.h"
#include "resources.h"
#include <raylib.h>
#include <raymath.h>

Vector2 hamster_pos = {40, -16}; // center
Vector2 hamster_velocity = {0};

short hamster_direction = 0;
float hamster_speed = 30.f;
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

    hamster_camera = (Camera2D){
        .zoom = 4.21f, .target = hamsterGetPosition(), .rotation = 0.f};
    hamsterCameraUpdate(); // to apply correct offset
}

void hamsterUpdate() {
    if (hamster_pos.y < -16.f) {
        hamster_is_grounded = false;
        hamster_velocity.y += hamster_gravity * GetFrameTime();
    } else {
        hamster_is_grounded = true;
        hamster_velocity.y = 0.f;
        hamster_pos.y = -16.f;
    }
    if (IsKeyPressed(KEY_UP) && hamster_is_grounded) {
        hamster_is_grounded = false;
        hamster_velocity.y = -hamster_jump_force;
    }

    hamster_direction = -IsKeyDown(KEY_LEFT) + IsKeyDown(KEY_RIGHT);
    switch (hamster_direction) {
    case -1:
        hamster_is_flipped = true;
        break;
    case 1:
        hamster_is_flipped = false;
        break;
    default:
        break;
    }
    hamster_anim->is_flipped = hamster_is_flipped;

    if (hamster_direction == 0) {
        hamster_anim = animations.hamster_idle;
    } else if (hamster_anim != animations.hamster_walk) {
        hamster_anim = animations.hamster_walk;
        animationReset(hamster_anim);
    }
    animationUpdate(hamster_anim);

    hamster_velocity.x += hamster_direction * hamster_speed *
                          (IsKeyDown(KEY_LEFT_SHIFT) + 1) * GetFrameTime();
    if (hamster_is_grounded)
        hamster_velocity.x = hamster_velocity.x / grass_friction;
    else
        hamster_velocity.x = hamster_velocity.x / air_friction;

    hamster_pos = Vector2Add(hamster_pos, hamster_velocity);
    hamster_camera.target = hamster_pos;
}

void hamsterCameraUpdate() {
    hamster_camera.offset.x = window_data.CENTER.x - 8 * hamster_camera.zoom;
    hamster_camera.offset.y = window_data.CENTER.y;
}

Camera2D *hamsterGetCamera() { return &hamster_camera; }

Vector2 hamsterGetPosition() { return hamster_pos; }

void hamsterDraw() {
    DrawTextureRec(textures.hamster, *animationGetFrame(hamster_anim),
                   hamster_pos, WHITE);
}
