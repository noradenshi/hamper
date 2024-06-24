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
Rectangle hamster_rec = (Rectangle){hamster_start_pos.x, hamster_start_pos.y,
                                    HAMSTER_SIZE, HAMSTER_SIZE};
Vector2 hamster_velocity = {0};
Vector2 hamster_direction = {0};

Vector2 hamster_ground_check;

short hamster_horizontal = 0;
const float hamster_speed = 25.f;
const float hamster_jump_force = 4.f;

const float hamster_max_speed = 10.f;
const float hamster_buffer_amount = .25f;
float hamster_jump_buffer_timer = 0.f;
float hamster_ground_buffer_timer = 0.f;

bool hamster_is_flipped = false;
bool hamster_is_grounded = false;

Animation *hamster_anim;
Camera2D hamster_camera;

// DEBUG
Collisions *colls;

// TEMP
const float air_friction = 5.f;
const float grass_friction = 15.f;

void hamsterInit() {
    hamster_anim = animations.hamster_idle;

    hamster_camera =
        (Camera2D){.zoom = 3.8f,
                   .target = (Vector2){hamster_rec.x, hamster_rec.y},
                   .rotation = 0.f};
    hamsterCameraUpdate(); // to apply correct offset
}

void hamsterReset() {
    hamster_rec = (Rectangle){hamster_start_pos.x, hamster_start_pos.y,
                              HAMSTER_SIZE, HAMSTER_SIZE};
    hamster_velocity = (Vector2){0.f, 0.f};
}

void hamsterSetPosition(float x, float y) {
    hamster_rec.x = x;
    hamster_rec.y = y;
}

float hamsterGetJumpBufferTime() { return hamster_jump_buffer_timer; }
float hamsterGetGroundBufferTime() { return hamster_ground_buffer_timer; }

Rectangle *hamsterGetRect() { return &hamster_rec; }

void hamsterHandleCollisions(Collisions *collisions) {
    colls = collisions;
    hamster_is_grounded = false;

    if (collisions->rec_y.height > .0f) {
        if (collisions->rec_y.y == hamster_rec.y) {
            hamster_rec.y += collisions->rec_y.height;
        } else {
            hamster_is_grounded = true;
            hamster_rec.y = collisions->rec_y.y - HAMSTER_SIZE;
        }
        hamster_velocity.y = .0f;
    }

    if (collisions->rec_x.width > .0f) {
        if (collisions->rec_x.x == hamster_rec.x) {
            hamster_rec.x += collisions->rec_x.width;
        } else {
            hamster_rec.x = collisions->rec_x.x - HAMSTER_SIZE;
        }
        hamster_velocity.x = .0f;
    }

    hamster_ground_check = (Vector2){hamster_rec.x + HAMSTER_SIZE / 2.f,
                                     hamster_rec.y + HAMSTER_SIZE};
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
        PlaySound(sounds.jump);
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
        hamster_jump_buffer_timer -= GetFrameTime();
    }

    if (hamster_is_grounded) {
        if (hamster_horizontal != 0 && !IsSoundPlaying(sounds.walk_grass))
            PlaySound(sounds.walk_grass);

        if (hamster_jump_buffer_timer > 0.f)
            hamsterJump();

        hamster_jump_buffer_timer = 0.f;
        hamster_velocity.x += ((hamster_velocity.x > 0) ? -1 : 1) *
                              grass_friction * GetFrameTime();
        hamster_ground_buffer_timer = hamster_buffer_amount;
    } else {
        hamster_velocity.y += levelGetGravity(active_level) * GetFrameTime();
        hamster_velocity.x -=
            ((hamster_velocity.x > 0) ? -1 : 1) * air_friction * GetFrameTime();
        hamster_ground_buffer_timer -= GetFrameTime();
    }

    // hamster_velocity = Vector2ClampValue(hamster_velocity,
    // -hamster_max_speed,
    //                                      hamster_max_speed);

    hamster_velocity.x = (hamster_velocity.x < hamster_max_speed)
                             ? hamster_velocity.x
                             : hamster_max_speed;

    hamster_rec.x += hamster_velocity.x;
    hamster_rec.y += hamster_velocity.y;

    hamster_camera.target = Vector2Lerp(
        hamster_camera.target, (Vector2){hamster_rec.x, hamster_rec.y}, .35f);

    hamsterCameraUpdate();
}

void hamsterCameraUpdate() {
    hamster_camera.offset.x = window_data.CENTER.x - 8 * hamster_camera.zoom;
    hamster_camera.offset.y = window_data.CENTER.y;
}

Camera2D *hamsterGetCamera() { return &hamster_camera; }

void hamsterDraw() {
    DrawTexturePro(textures.players, *animationGetFrame(hamster_anim),
                   hamster_rec, Vector2Zero(), 0.f, WHITE);

    if (IS_DEBUG) {
        if (colls != nullptr) {
            DrawRectangleRec(colls->rec_y, RED);
            DrawRectangleRec(colls->rec_x, RED);
        }

        DrawCircleV(hamster_ground_check, 1.f, BLUE);
        if (hamster_is_grounded)
            DrawCircle(hamster_rec.x + 8.f, hamster_rec.y + 20.f, 1, GREEN);
    }
}

void hamsterDrawHUD() {
    if (colls == nullptr)
        return;

    const char *text_y =
        TextFormat("rec_y: %.2f %.2f %.2f %.2f", colls->rec_y.x, colls->rec_y.y,
                   colls->rec_y.width, colls->rec_y.height);
    const char *text_x =
        TextFormat("rec_x: %.2f %.2f %.2f %.2f", colls->rec_x.x, colls->rec_x.x,
                   colls->rec_x.width, colls->rec_x.height);

    DrawText(text_x, window_data.WIDTH - MeasureText(text_x, 20),
             window_data.HEIGHT - 25 * 2, 20, WHITE);
    DrawText(text_y, window_data.WIDTH - MeasureText(text_y, 20),
             window_data.HEIGHT - 25, 20, WHITE);
}
