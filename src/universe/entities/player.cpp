#include "../entity.hpp"
#include "../../game.h"


float max_render_scale = 2.0f;
float min_render_scale = 0.5f;


PlayerCharacter::PlayerCharacter(EntityData *_data) : CreatureEntity(_data) {

    //entity_data = _data;
    
    y_sort = true;
    is_on_screen = true;
    is_stunned = false;


    /* CharacterControllerTemplateData char_Tdata = g_character_controller_template_data[entity_data->character_controller_id];
    character_controller_data.id = char_Tdata.id;
    character_controller_data.name = char_Tdata.name;
    character_controller_data.movement = char_Tdata.movement; */


    //ship_data = &;
    /* ShipTemplateData ship_Tdata = g_ship_template_data[entity_data->ship_id];

    ship_data.id = ship_Tdata.id;
    ship_data.name = ship_Tdata.name;
    ship_data.uid = GetUID();
    ship_data.value = ship_Tdata.value;
    ship_data.radius = ship_Tdata.radius;
    ship_data.fuel = 100;
 */

    //ship_controller_data.flight_modes[1] = ship_Tdata.planet_drive;

    //ship_controller_data.name = ship_Tdata.name;
    //ship_controller_data.value = ship_Tdata.value;

    //ship_controller_data.radius = ship_Tdata.radius;  //rendering data

    //printf("SHIP data  entity id: %i   ship id: %i  radius: %0.3f\n", entity_data->id, entity_data->ship_id, ship_data->radius);

    //EnterShip();

}

/* PlayerCharacter::~PlayerCharacter() {
    if (selection_manager){
        selection_manager->Unregister(&info_area);
    }

} */

void PlayerCharacter::Update() {

    UpdateMovement();
}

void PlayerCharacter::Draw() {

    //DrawCircleV({entity_data->position.x, entity_data->position.y}, 20, PINK);
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


void PlayerCharacter::EnterShip(ShipData *_data) {
    ship_data = _data;
    character.reset();
    
    printf("entering ship   equimnent tags: %i\n", ship_data->equipment_tags.size());
    ship = std::make_unique<Ship>(_data);

    
    /* ship_controller = std::make_unique<ShipController>(_data);
    ship_controller->SetFlightMode(LOCAL_FLIGHT_MODE);
    ship_controller->flight_modes[LOCAL_FLIGHT_MODE].velocity = {0,0};
    ship_controller->flight_modes[LOCAL_FLIGHT_MODE].throttle = 0.0f; */

    movement_type = MOVEMENT_SHIP;
    printf("enter ship\n");

}

void PlayerCharacter::ExitShip(CharacterData *_data) {
    character_data = _data;
    ship.reset();
    character = std::make_unique<Character>(character_data);
    character->character_controller->movement.velocity = {0,0};
    
    movement_type = MOVEMENT_CHARACTER;
    printf("exit ship\n");
}


