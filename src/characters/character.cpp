#include "characters.hpp"
#include "../game.h"

Character::Character(CharacterData *_data) {

    character_data = _data;

    character_controller = std::make_unique<CharacterController>(_data);
    //ship_controller->flight_modes[LOCAL_FLIGHT_MODE].velocity = {0,0};
    //ship_controller->flight_modes[LOCAL_FLIGHT_MODE].throttle = 0.0f;

    }

void Character::Update(Vector2 &position) {
    character_controller->Update(position);
}

void Character::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    DrawCircleV(screen, character_data->radius * scale, PINK);
    //character_controller->Draw(position, scale);

}