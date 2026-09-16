#include "../entity.hpp"
#include "../../game.h"


float max_render_scale = 2.0f;
float min_render_scale = 0.25f;


PlayerCharacter::PlayerCharacter(EntityData *_data) : CreatureEntity(_data) {

    
    y_sort = true;
    is_on_screen = true;
    is_stunned = false;


}


void PlayerCharacter::Update() {

    UpdateMovement();
}

void PlayerCharacter::Draw() {

    if (movement_type == MOVEMENT_SHIP and ship != nullptr) {
        ship->Draw(entity_data->position);
    } else if (movement_type == MOVEMENT_CHARACTER and character != nullptr) {
        character->Draw(entity_data->position);
    }
    DrawCircleV({entity_data->position.x, entity_data->position.y}, 20, PINK);
}



void PlayerCharacter::DrawOverlay() {

    float scale = GetRenderScale();
    if (movement_type == MOVEMENT_SHIP and ship != nullptr) {
        ship->Draw(entity_data->position, scale);
        //ship->Draw();
    } else if (movement_type == MOVEMENT_CHARACTER and character != nullptr) {
        character->Draw(entity_data->position, scale);
    }
    else {
        TraceLog(LOG_INFO, "PlayerCharacter::DrawOverlay() movement_type is invalid or ship/character is null");
    }

}

void PlayerCharacter::DrawUI() {

}


void PlayerCharacter::Die() {
    TraceLog(LOG_INFO, "Player has died");
}


void PlayerCharacter::UpdateMovement() {

    if (movement_type == MOVEMENT_SHIP and ship != nullptr) {
        ship->Update(entity_data->position);
    } else if (movement_type == MOVEMENT_CHARACTER and character != nullptr) {
        character->Update(entity_data->position);
        
    }
    else {
        TraceLog(LOG_INFO, "PlayerCharacter::UpdateMovement() movement_type is invalid or ship/character is null");
    }
}



float PlayerCharacter::GetRenderScale()
{
    if(entity_data->render_mode == RENDER_WORLD)
        return g_camera.zoom;

    if(entity_data->render_mode == RENDER_CAPPED) {

        if(g_camera.zoom > max_render_scale) {
            return max_render_scale;
        }
        if(g_camera.zoom < min_render_scale) {
            return min_render_scale;
        }

        return g_camera.zoom;
    }

    return 1.0f;
}


void PlayerCharacter::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
}


void PlayerCharacter::EnterShip(ShipData *_data) {
    ship_data = _data;
    character.reset();
    
    printf("entering ship   equimnent tags: %i\n", ship_data->equipment_tags.size());
    ship = std::make_unique<Ship>(_data);
    movement_type = MOVEMENT_SHIP;
    printf("enter ship\n");

}

void PlayerCharacter::ExitShip(CharacterData *_data) {
    character_data = _data;
    ship.reset();

    character = std::make_unique<Character>(character_data);
    movement_type = MOVEMENT_CHARACTER;
    printf("exit ship\n");
}


