#include "../controllers.hpp"
#include "../../game.h"

CharacterController::CharacterController(CharacterData *_data) {

    character_data = _data;
    movement.speed = character_data->speed;
}

CharacterController::~CharacterController() {

}

void CharacterController::Update(Vector2 &position) {

    Vector2 input_dir = {0,0};

    if(g_input.key_up) {input_dir.y = -1;}
    if(g_input.key_down) {input_dir.y = 1;}
    if(g_input.key_left) {input_dir.x = -1;}
    if(g_input.key_right) {input_dir.x = 1;}    

    Vector2 next_position = position;

    if(movement.velocity.x != 0 or movement.velocity.y != 0 ) {
        next_position = Vector2Add(next_position, movement.velocity * GetFrameTime());
    }

    position = next_position;

    float speed =  movement.speed;

    if( isnan(movement.velocity.x) || isnan(movement.velocity.y)) {movement.velocity = {0,0};}
        
    movement.velocity = Vector2Lerp(movement.velocity, input_dir * speed, .15);

    if(abs(movement.velocity.x) < 4.0f) {
        movement.velocity.x = {0.0};
    }
    if (abs(movement.velocity.y) < 4.0f) {
        movement.velocity.y = {0.0};
    }
/*     printf("character movement\n");
    printf("input %.1f %.1f  velocity %.1f %.1f  position %.1f %.1f speed %0.4f\n",
    input_dir.x,
    input_dir.y,
    character_data->movement.velocity.x,
    character_data->movement.velocity.y,
    position.x,
    position.y,
    speed); */
}

void CharacterController::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);    
    DrawCircleV(screen, 16 * scale, RAYWHITE);

}

