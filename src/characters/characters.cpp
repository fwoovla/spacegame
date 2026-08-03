#include "characters.hpp"
#include "../game.h"

Character::Character(CharacterData _data) {

    character_data = _data;
    
}

Character::~Character() {

}

void Character::Update(Vector2 &position) {


 Vector2 input_dir = {0,0};


    if(g_input.key_up) {input_dir.y = -1;}
    if(g_input.key_down) {input_dir.y = 1;}
    if(g_input.key_left) {input_dir.x = -1;}
    if(g_input.key_right) {input_dir.x = 1;}    

    Vector2 next_position = position;

    if(character_data.movement.velocity.x != 0 or character_data.movement.velocity.y != 0 ) {
        next_position = Vector2Add(next_position, character_data.movement.velocity * GetFrameTime());
    }

    position = next_position;

    float speed =  character_data.movement.speed;

    if( isnan(character_data.movement.velocity.x) || isnan(character_data.movement.velocity.y)) {character_data.movement.velocity = {0,0};}
        
    character_data.movement.velocity = Vector2Lerp(character_data.movement.velocity, input_dir * speed, .15);

    if(abs(character_data.movement.velocity.x) < 4.0f) {
        character_data.movement.velocity.x = {0.0};
    }
    if (abs(character_data.movement.velocity.y) < 4.0f) {
        character_data.movement.velocity.y = {0.0};
    }

}

void Character::Draw() {

}

