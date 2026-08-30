#include "../entity.hpp"
#include "../../game.h"


float max_render_scale = 2.0f;
float min_render_scale = 0.5f;


PlayerCharacter::PlayerCharacter(EntityData *_data) {

    entity_data = _data;
    
    y_sort = true;
    is_on_screen = true;
    is_stunned = false;

    CharacterControllerTemplateData char_Tdata = g_character_controller_template_data[entity_data->character_controller_id];
    character_controller_data.id = char_Tdata.id;
    character_controller_data.name = char_Tdata.name;
    character_controller_data.movement = char_Tdata.movement;


    ShipControllerTemplateData ship_Tdata = g_ship_controller_template_data[entity_data->ship_controller_id];
    ship_controller_data.flight_modes[0] = ship_Tdata.system_drive;
    ship_controller_data.flight_modes[1] = ship_Tdata.planet_drive;

    ship_controller_data.name = ship_Tdata.name;
    ship_controller_data.value = ship_Tdata.value;

    ship_controller_data.radius = ship_Tdata.radius;  //rendering data

    printf("SHIP data  entity id: %i   controller id: %i  radius: %0.3f\n", entity_data->id, entity_data->ship_controller_id, ship_controller_data.radius);

    EnterShip();

}

PlayerCharacter::~PlayerCharacter() {

}

void PlayerCharacter::Update() {

    UpdateMovement();
}

void PlayerCharacter::Draw() {

    //DrawCircleV({entity_data->position.x, entity_data->position.y}, 20, PINK);
}



void PlayerCharacter::DrawOverlay() {

    float scale = GetRenderScale();
    if (movement_type == MOVEMENT_SHIP and ship_controller != nullptr) {
        ship_controller->Draw(entity_data->position, scale);
    } else if (movement_type == MOVEMENT_CHARACTER and character_controller != nullptr) {
        character_controller->Draw(entity_data->position, scale);
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

    if (movement_type == MOVEMENT_SHIP and ship_controller != nullptr) {
        ship_controller->Update(entity_data->position);
    } else if (movement_type == MOVEMENT_CHARACTER and character_controller != nullptr) {
        character_controller->Update(entity_data->position);
        
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
    
    character_controller.reset();
    ship_controller = std::make_unique<ShipController>(&ship_controller_data);
    ship_controller->SetFlightMode(LOCAL_FLIGHT_MODE);
    ship_controller->ship_data->flight_modes[LOCAL_FLIGHT_MODE].velocity = {0,0};
    ship_controller->ship_data->flight_modes[LOCAL_FLIGHT_MODE].throttle = 0.0f;

    movement_type = MOVEMENT_SHIP;
    printf("enter ship\n");

}

void PlayerCharacter::ExitShip() {

    ship_controller.reset();
    character_controller = std::make_unique<CharacterController>(&character_controller_data);
    character_controller->character_data->movement.velocity = {0,0};
    
    movement_type = MOVEMENT_CHARACTER;
    printf("exit ship\n");
}


