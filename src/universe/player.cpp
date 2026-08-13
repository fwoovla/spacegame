#include "entity.hpp"
#include "../game.h"


float max_render_scale = 2.0f;
float min_render_scale = 0.5f;


PlayerCharacter::PlayerCharacter(EntityData *_data) {

    entity_data = _data;
    
    y_sort = true;
    is_on_screen = true;
    is_stunned = false;
    
    velocity = {0,0};

    ShipTemplateData ship_Tdata = g_ship_template_data[SHIP_1];
    ShipData ship_data;
    ship_data.id = ship_Tdata.id;
    ship_data.name = ship_Tdata.name;
    ship_data.value = ship_Tdata.value;
    ship_data.flight_modes[0] = ship_Tdata.system_drive;
    ship_data.flight_modes[1] = ship_Tdata.planet_drive;

    ship = std::make_unique<Ship>(ship_data);

}

PlayerCharacter::~PlayerCharacter() {

}

void PlayerCharacter::Update() {

    UpdateMovement();
}

void PlayerCharacter::Draw() {

    DrawCircleV({entity_data->position.x, entity_data->position.y}, 20, PINK);
}



void PlayerCharacter::DrawOverlay() {

    float scale = GetRenderScale();
    if (movement_type == MOVEMENT_SHIP and ship != nullptr) {
        ship->Draw(entity_data->position, scale);
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
    //g_game_data.running = false;
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


void PlayerCharacter::EnterShip() {
    

    ShipTemplateData ship_Tdata = g_ship_template_data[SHIP_1];
    ShipData ship_data;
    ship_data.id = ship_Tdata.id;
    ship_data.name = ship_Tdata.name;
    ship_data.flight_modes[SYSTEM_FLIGHT_MODE] = ship_Tdata.system_drive;
    ship_data.flight_modes[PLANET_FLIGHT_MODE] = ship_Tdata.planet_drive;

    g_current_player->ship = std::make_unique<Ship>(ship_data);

    movement_type = MOVEMENT_SHIP;

}

void PlayerCharacter::ExitShip() {

    CharacterTemplateData char_Tdata = g_character_template_data[CHARACTER_PLAYER];
    CharacterData char_data;
    char_data.id = char_Tdata.id;
    char_data.name = char_Tdata.name;
    char_data.movement = char_Tdata.movement;

    g_current_player->character = std::make_unique<Character>(char_data);
    movement_type = MOVEMENT_CHARACTER;
}


