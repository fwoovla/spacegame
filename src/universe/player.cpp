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
    ship_data.movement = ship_Tdata.movement;

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

    Vector2 screen = GetWorldToScreen2D(entity_data->position, g_camera);
    Vector2 forward = {cosf(ship->ship_data.movement.rotation) * 100.0f, 
        sinf(ship->ship_data.movement.rotation) * 100.0f};

    forward = Vector2Add(screen, forward);
    
    DrawCircleV(screen, 20 * scale, PINK);
    DrawLineV(screen, forward, RED);
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
        //UpdateShipMovement();
    } else if (movement_type == MOVEMENT_CHARACTER and character != nullptr) {
        character->Update(entity_data->position);
        //UpdateCharacterMovement();
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


void PlayerCharacter::EnterShip() {
    

    ShipTemplateData ship_Tdata = g_ship_template_data[SHIP_1];
    ShipData ship_data;
    ship_data.id = ship_Tdata.id;
    ship_data.name = ship_Tdata.name;
    ship_data.movement = ship_Tdata.movement;

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


/* void PlayerCharacter::UpdateShipMovement() {
    if(ship == nullptr) {
        TraceLog(LOG_INFO, "PlayerCharacter::UpdateShipMovement() ship is null");
        return;
    }

    ShipData *ship_data = &ship.get()->ship_data;


    float dt = GetFrameTime();
    printf("dt %0.5f\n", dt);

    if(g_input.key_left) ship_data->movement.rotation -= ship_data->movement.turn_speed * dt;
    if(g_input.key_right) ship_data->movement.rotation += ship_data->movement.turn_speed * dt;  


    Vector2 forward ={cosf(ship_data->movement.rotation), sinf(ship_data->movement.rotation)};

    if(g_input.key_up) { 
        ship_data->movement.velocity.x += forward.x * ship_data->movement.thrust * dt;
        ship_data->movement.velocity.y += forward.y * ship_data->movement.thrust * dt;
    }


    if(g_input.key_down) {
        ship_data->movement.velocity.x -= forward.x * ship_data->movement.reverse_thrust * dt;
        ship_data->movement.velocity.y -= forward.y * ship_data->movement.reverse_thrust * dt;
    }

    float speed = Vector2Length(ship_data->movement.velocity);

    if(speed > ship_data->movement.max_speed) {
        ship_data->movement.velocity = Vector2Scale( Vector2Normalize(ship_data->movement.velocity), ship_data->movement.max_speed);
    }

    ship_data->movement.velocity.x *= 1.0f - ship_data->movement.drag * dt;
    ship_data->movement.velocity.y *= 1.0f - ship_data->movement.drag * dt;

    entity_data->position.x += ship_data->movement.velocity.x * dt;
    entity_data->position.y += ship_data->movement.velocity.y * dt;


    printf("player pos %0.5f %0.5f\n", entity_data->position.x, entity_data->position.y);
    printf("player vel %0.5f %0.5f\n", ship_data->movement.velocity.x, ship_data->movement.velocity.y);
    printf("player speed %0.5f\n", Vector2Length(ship_data->movement.velocity));
    printf("player heading %0.5f\n", ship_data->movement.rotation);
    printf("player throttle %0.5f\n", ship_data->movement.throttle);
    printf("player thrust %0.5f\n\n\n", ship_data->movement.thrust);

} 
 */


/* void PlayerCharacter::UpdateCharacterMovement() {
    if(character == nullptr) {
        TraceLog(LOG_INFO, "PlayerCharacter::UpdateCharacterMovement() character is null");
        return;
    }

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

    float speed =  character->character_data.movement.speed;

    if( isnan(velocity.x) || isnan(velocity.y)) {velocity = {0,0};}
        
    velocity = Vector2Lerp(velocity, input_dir * speed, .15);

    if(abs(velocity.x) < 4.0f) {
        velocity.x = {0.0};
    }
    if (abs(velocity.y) < 4.0f) {
        velocity.y = {0.0};
    }

}  */