#include "entity.hpp"
#include "../game.h"

PlayerCharacter::PlayerCharacter(EntityData *_data) {

    entity_data = _data;
    
    y_sort = true;
    is_on_screen = true;
    is_stunned = false;
    
    velocity = {0,0};   
}

PlayerCharacter::~PlayerCharacter() {

}

void PlayerCharacter::Update() {


    UpdateMovement();


}

void PlayerCharacter::Draw() {

    //TraceLog(LOG_INFO, "drawing player %i", data.current_layer);
    //DrawSprite(sprite);
    DrawCircleV({entity_data->position.x, entity_data->position.y}, 20, PINK);
}

void PlayerCharacter::DrawUI() {

}


void PlayerCharacter::Die() {
    TraceLog(LOG_INFO, "Player has died");
    //g_game_data.running = false;
}


void PlayerCharacter::UpdateMovement() {

    Vector2 input_dir = {0,0};


    if(g_input.key_up) {input_dir.y = -1;}
    
    if(g_input.key_down) {input_dir.y = 1;}
    
    if(g_input.key_left) {input_dir.x = -1;}
    
    if(g_input.key_right) {input_dir.x = 1;}    
    
    Vector2 next_position = entity_data->position;


    if(velocity.x != 0 or velocity.y != 0 ) {
        next_position = Vector2Add(next_position, velocity * GetFrameTime());
    }

    entity_data->position = next_position;

            

    

/*     else if(!is_stunned) {
            SetAmination(sprite, ANIM_IDLE);
    } */
    


    float speed = entity_data->movement.speed;

    if( isnan(velocity.x) || isnan(velocity.y)) {velocity = {0,0};}
        
    velocity = Vector2Lerp(velocity, input_dir * speed, .15);

    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }


    //printf("player velocirty:  %0.3f %0.3f  input dir  %i  %i  speed: %0.5f\n", velocity.x, velocity.y, (int)input_dir.x, (int)input_dir.y, speed);

}


