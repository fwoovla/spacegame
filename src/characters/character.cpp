#include "characters.hpp"
#include "../game.h"

Character::Character(CharacterData *_data) {
    character_data = _data;
    character_controller = std::make_unique<CharacterController>(_data);
}

void Character::Update(Vector2 &position) {
    character_controller->Update(position);
}

void Character::Draw(Vector2 &position, float scale) {
    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    DrawCircleV(screen, character_data->radius * scale, RAYWHITE);
}

void Character::Draw(Vector2 &position) {
    DrawCircleV({position.x, position.y}, 20, PINK);
}