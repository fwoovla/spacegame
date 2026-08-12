#include "../game.h"
#include "ships.hpp"


Ship::Ship(ShipData _data) {
    ship_data = _data;
}

Ship::~Ship() {

}

void Ship::Update(Vector2 &position) {

    float dt = GetFrameTime();
    //printf("dt %0.5f\n", dt);

    if(g_input.key_left) ship_data.movement.rotation -= ship_data.movement.turn_speed * dt;
    if(g_input.key_right) ship_data.movement.rotation += ship_data.movement.turn_speed * dt;  


    Vector2 forward ={cosf(ship_data.movement.rotation), sinf(ship_data.movement.rotation)};

    if(g_input.key_up) { 
        ship_data.movement.throttle = 1.0f;
        ship_data.movement.throttle_override = true;
        //ship_data.movement.velocity.x += forward.x * ship_data.movement.thrust * ship_data.movement.throttle * dt;
        //ship_data.movement.velocity.y += forward.y * ship_data.movement.thrust * ship_data.movement.throttle * dt;
    }
    else if(g_input.key_down) {
        ship_data.movement.throttle = -1.0f;
        ship_data.movement.throttle_override = true;

        //ship_data.movement.velocity.x -= forward.x * ship_data.movement.reverse_thrust * dt;
        //ship_data.movement.velocity.y -= forward.y * ship_data.movement.reverse_thrust * dt;
    }
    else {
        if(ship_data.movement.throttle_override) {
            ship_data.movement.throttle = 0.0f;
            ship_data.movement.throttle_override = false;
        }
    }

    if(g_input.key_throttle_up and !ship_data.movement.throttle_override) {
        ship_data.movement.throttle += 0.1f * dt;
        if(ship_data.movement.throttle > 1.0f) ship_data.movement.throttle = 1.0f;
    }
    if(g_input.key_throttle_down and !ship_data.movement.throttle_override) {
        ship_data.movement.throttle -= 0.1f * dt;
        if(ship_data.movement.throttle < -1.0f) ship_data.movement.throttle = -1.0f;
    }


    ship_data.movement.velocity.x += forward.x * ship_data.movement.thrust * ship_data.movement.throttle * dt;
    ship_data.movement.velocity.y += forward.y * ship_data.movement.thrust * ship_data.movement.throttle * dt;


    float speed = Vector2Length(ship_data.movement.velocity);

    if(speed > ship_data.movement.max_speed) {
        ship_data.movement.velocity = Vector2Scale( Vector2Normalize(ship_data.movement.velocity), ship_data.movement.max_speed);
    }

    ship_data.movement.velocity.x *= 1.0f - ship_data.movement.drag * dt;
    ship_data.movement.velocity.y *= 1.0f - ship_data.movement.drag * dt;

    position.x += ship_data.movement.velocity.x * dt;
    position.y += ship_data.movement.velocity.y * dt;

/* 
    printf("player pos %0.5f %0.5f\n", position.x, position.y);
    printf("player vel %0.5f %0.5f\n", ship_data.movement.velocity.x, ship_data.movement.velocity.y);
    printf("player speed %0.5f\n", Vector2Length(ship_data.movement.velocity));
    printf("player heading %0.5f\n", ship_data.movement.rotation);
    printf("player throttle %0.5f\n", ship_data.movement.throttle);
    printf("player thrust %0.5f\n\n\n", ship_data.movement.thrust);
 */
}

void Ship::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    Vector2 forward = {cosf(ship_data.movement.rotation) * 100.0f, 
        sinf(ship_data.movement.rotation) * 100.0f};

    forward = Vector2Add(screen, forward);
    
    DrawCircleV(screen, 20 * scale, PINK);
    DrawLineV(screen, forward, RED);
}


bool Ship::ToggleAutoPilot() {
    ship_data.movement.autopiolot_on = !ship_data.movement.autopiolot_on;
    return ship_data.movement.autopiolot_on;

}

bool Ship::ToggleFlightAssist() {
    ship_data.movement.flight_assist_on = !ship_data.movement.flight_assist_on;
    return ship_data.movement.flight_assist_on;
}