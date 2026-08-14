#include "../game.h"
#include "ships.hpp"


Ship::Ship(ShipData _data) {
    ship_data = _data;
    ship_data.flight_mode = SYSTEM_FLIGHT_MODE;
    current_mode = &ship_data.flight_modes.at(ship_data.flight_mode);
    autopilot.flight_mode = current_mode;
}

Ship::~Ship() {

}

void Ship::Update(Vector2 &position) {
    float dt = GetFrameTime();

    if(flight_assist_on) {
        float target_angle = GetAngleFromTo(position, g_input.world_mouse_position);
        current_mode->rotation = RotateTowardsRad(current_mode->rotation, target_angle, PI, dt );

        current_mode->throttle = Vector2DistanceSqr(position, g_input.world_mouse_position) * 0.00001f * (g_camera.zoom);
        if(current_mode->throttle > 1.0f) current_mode->throttle = 1.0f;
        //printf("%0.5f\n", Vector2DistanceSqr(position, g_input.world_mouse_position) * 0.00001f * (g_camera.zoom * 0.5f));

    }
    else if(autopilot_on) {
        AutopilotInput ap_input;
        ap_input.position = position;
        ap_input.rotation = current_mode->rotation;
        ap_input.velocity = current_mode->velocity;
        FlightInput f_input = autopilot.Update(ap_input, dt);

        current_mode->rotation = f_input.turn;
        current_mode->throttle = f_input.throttle;
        if(autopilot.state == ARRIVE) {
            AutopilotTarget dummy;
            ToggleAutoPilot(dummy);
        }
        
        //printf("ap update?\n");

    }
    else {
        ManualFlightInput(dt);
    }
    FlightUpdate(position, dt);
}

void Ship::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    Vector2 forward = {cosf(current_mode->rotation) * 100.0f, sinf(current_mode->rotation) * 100.0f};

    forward = Vector2Add(screen, forward);
    
    DrawCircleV(screen, 20 * scale, PINK);
    DrawLineV(screen, forward, RED);

    if(autopilot_on) {
        DrawLineV(screen, Vector2Add(screen, Vector2Scale(autopilot.to_target, 100.0f)), ORANGE);
        DrawLineV(screen, Vector2Add(screen, Vector2Scale(current_mode->velocity, 0.1f)), GREEN);
        DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.target_velocity, 0.1f)), 5, BLUE);
        //DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.velocity_error, 0.1f)), 5, PURPLE);
        //DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.lateral_velocity, 0.1f)), 5, LIME);
        DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.to_target, current_mode->throttle * 100.0f)), 5, MAGENTA);

    }

}


bool Ship::ToggleAutoPilot(AutopilotTarget &target) {
    autopilot_on = !autopilot_on;
    if(!autopilot_on) {
        target = {};
    }
    printf("autopilot %i\n", autopilot_on);
    autopilot.SetTarget(target);

    return autopilot_on;

}

bool Ship::ToggleFlightAssist() {
    flight_assist_on = !flight_assist_on;
    return flight_assist_on;
}

void Ship::SetFlightMode(FLIGHT_MODE mode) {
    ship_data.flight_mode = mode;
    
    ship_data.flight_modes[ship_data.flight_mode].velocity = current_mode->velocity;
    ship_data.flight_modes[ship_data.flight_mode].rotation = current_mode->rotation;
    ship_data.flight_modes[ship_data.flight_mode].throttle = current_mode->throttle;

    current_mode = &ship_data.flight_modes.at(ship_data.flight_mode);
    autopilot.flight_mode = current_mode;
}

void Ship::AutopilotUpdate(Vector2 position) {
    //autopilot.Update();
}

void Ship::FlightAssistUpdateUpdate(Vector2 &position) {

}

void Ship::ManualFlightInput(float dt) {
    

    if(g_input.key_left) current_mode->rotation -= current_mode->turn_speed * dt;
    if(g_input.key_right) current_mode->rotation += current_mode->turn_speed * dt;  

    Vector2 forward ={cosf(current_mode->rotation), sinf(current_mode->rotation)};
    if(g_input.key_up) { 
        current_mode->throttle = 1.0f;
        current_mode->throttle_override = true;
    }
    else if(g_input.key_down) {
        current_mode->throttle = -1.0f;
        current_mode->throttle_override = true;
    }
    else {
        if(current_mode->throttle_override) {
            current_mode->throttle = 0.0f;
            current_mode->throttle_override = false;
        }
    }
    if(g_input.key_throttle_up and !current_mode->throttle_override) {
        current_mode->throttle += 0.1f * dt;
        if(current_mode->throttle > 1.0f) current_mode->throttle = 1.0f;
    }
    if(g_input.key_throttle_down and !current_mode->throttle_override) {
        current_mode->throttle -= 0.1f * dt;
        if(current_mode->throttle < -1.0f) current_mode->throttle = -1.0f;
    }
    

}


void Ship::FlightUpdate(Vector2 &position, float dt) {


    float thrust = current_mode->thrust;
    if(current_mode->throttle < 0.0f) {
        thrust = current_mode->reverse_thrust;
    }

    Vector2 forward ={cosf(current_mode->rotation), sinf(current_mode->rotation)};
    current_mode->velocity.x += forward.x * thrust * current_mode->throttle * dt;
    current_mode->velocity.y += forward.y * thrust * current_mode->throttle * dt;
    float speed = Vector2Length(current_mode->velocity);
    if(speed > current_mode->max_speed) {
        current_mode->velocity = Vector2Scale( Vector2Normalize(current_mode->velocity), current_mode->max_speed);
    }
    current_mode->velocity.x *= 1.0f - current_mode->drag * dt;
    current_mode->velocity.y *= 1.0f - current_mode->drag * dt;
    position.x += current_mode->velocity.x * dt;
    position.y += current_mode->velocity.y * dt;
}