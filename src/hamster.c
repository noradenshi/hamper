#include "hamster.h"
#include "alley.h"
#include "animation.h"
#include "gamestate.h"
#include "resources.h"
#include "tilemap.h"
#include <raylib.h>
#include <raymath.h>

#define HAMSTER_SIZE 16

const Vector2 hamster_start_pos = {40, -36}; // center
Vector2 hamster_pos = hamster_start_pos;
Vector2 hamster_velocity = {0};
Vector2 hamster_direction = {0};

Vector2 hamster_ground_check;

short hamster_horizontal = 0;
float hamster_speed = 25.f;
float hamster_jump_force = 4.f;
float hamster_gravity = 8.f;

const float hamster_max_speed = 100.f;
const float hamster_buffer_amount = .25f;
float hamster_jump_buffer_timer = 0.f;
float hamster_ground_buffer_timer = 0.f;

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
        (Camera2D){.zoom = 3.8f, .target = hamster_pos, .rotation = 0.f};
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

float hamsterGetJumpBufferTime() { return hamster_jump_buffer_timer; }
float hamsterGetGroundBufferTime() { return hamster_ground_buffer_timer; }

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

    hamster_ground_check = (Vector2){hamster_pos.x + HAMSTER_SIZE / 2.f,
                                     hamster_pos.y + HAMSTER_SIZE};
    if (hamster_velocity.y >= 0.f &&
        alleyLineCheckCollisions(&hamster_ground_check)) {

        hamster_is_grounded = true;

        hamster_velocity =
            Vector2Reflect(hamster_velocity, alleyLineGetNormal());

        // TODO: first bounce should go higher
        hamster_velocity.y *= .6f;
        if (hamster_velocity.y > 0.f)
            hamster_velocity.y = -.1f;
    }

    hamsterCameraUpdate();
}

void hamsterMove(short direction) {
    hamster_horizontal = direction;
    if (direction != 0) {
        hamster_is_flipped = (direction == -1);
        animationSetFlipped(hamster_anim, hamster_is_flipped);
    }

    hamster_velocity.x += hamster_horizontal * hamster_speed * GetFrameTime();
}

void hamsterJump() {
    if (hamster_is_grounded || hamster_ground_buffer_timer > 0.f) {
        hamster_is_grounded = false;
        hamster_velocity.y = -hamster_jump_force;
        hamster_ground_buffer_timer = 0.f;
        return;
    }

    hamster_jump_buffer_timer = hamster_buffer_amount;
}

void hamsterUpdate() {
    if (hamster_horizontal == 0) {
        hamster_anim = animations.hamster_idle;
        StopSound(sounds.walk_grass);
    } else if (hamster_anim != animations.hamster_walk) {
        hamster_anim = animations.hamster_walk;
        animationReset(hamster_anim);
    }
    animationUpdate(hamster_anim);

    if (hamster_jump_buffer_timer > 0.f) {
        hamster_jump_buffer_timer -= GetFrameTime() * 1.f;
    }

    if (hamster_is_grounded) {
        if (hamster_horizontal != 0 && !IsSoundPlaying(sounds.walk_grass))
            PlaySound(sounds.walk_grass);

        if (hamster_jump_buffer_timer > 0.f)
            hamsterJump();

        hamster_jump_buffer_timer = 0.f;
        hamster_velocity.x = hamster_velocity.x / grass_friction;
        hamster_ground_buffer_timer = hamster_buffer_amount;
    } else {
        hamster_velocity.y += hamster_gravity * GetFrameTime();
        hamster_velocity.x = hamster_velocity.x / air_friction;
        hamster_ground_buffer_timer -= GetFrameTime();
    }

    hamster_velocity = Vector2ClampValue(hamster_velocity, -hamster_max_speed,
                                         hamster_max_speed);

    hamster_pos = Vector2Add(hamster_pos, hamster_velocity);
    hamster_camera.target =
        Vector2Lerp(hamster_camera.target, hamster_pos, .35f);

    // hamsterCameraUpdate();
}

void hamsterCameraUpdate() {
    hamster_camera.offset.x = window_data.CENTER.x - 8 * hamster_camera.zoom;
    hamster_camera.offset.y = window_data.CENTER.y;
}

Camera2D *hamsterGetCamera() { return &hamster_camera; }

void hamsterDraw() {
    DrawTextureRec(textures.players, *animationGetFrame(hamster_anim),
                   hamster_pos, WHITE);

    if (IS_DEBUG) {
        DrawCircleV(hamster_ground_check, 1.f, BLUE);
        if (hamster_is_grounded)
            DrawCircle(hamster_pos.x + 8.f, hamster_pos.y + 20.f, 1, GREEN);
    }
}
