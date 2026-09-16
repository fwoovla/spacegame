#include "../controllers.hpp"
#include "../../game.h"

ShipController::ShipController(ShipData *_data) {
    ship_data = _data;

    printf("setting fm\n");

    SetFlightModes();

    current_mode = &flight_modes.at(flight_mode);
    autopilot.flight_mode = current_mode;
}

ShipController::~ShipController() {

}

void ShipController::Update(Vector2 &position) {
    float dt = GetFrameTime();

    current_mode->speed = Vector2Length(current_mode->velocity);

    if(flight_assist_on and !g_input.in_use) {
        float target_angle = GetAngleFromTo(position, g_input.world_mouse_position);
        current_mode->rotation = RotateTowardsRad(current_mode->rotation, target_angle, PI, dt );

        current_mode->throttle = Vector2DistanceSqr(position, g_input.world_mouse_position) * 0.00001f * (g_camera.zoom);
        if(current_mode->throttle > 1.0f) current_mode->throttle = 1.0f;

    }
    else if(autopilot_on and autopilot.target_data.set) {
        
        if(Vector2Distance(position, autopilot.target_data.position) < autopilot.target_data.proximity_radius and flight_mode != LOCAL_FLIGHT_MODE) {
            SetFlightMode(LOCAL_FLIGHT_MODE);
            //autopilot.enter_local_space.EmitSignal();
            //autopilot.state = ACCELERATE;
        }

        AutopilotInput ap_input;
        ap_input.position = position;
        ap_input.rotation = current_mode->rotation;
        ap_input.velocity = current_mode->velocity;
        FlightInput f_input = autopilot.Update(ap_input, dt);

        if(autopilot.state == ARRIVE) {
            flight_mode = LOCAL_FLIGHT_MODE;
            current_mode =  &flight_modes.at(flight_mode);
            current_mode->rotation = f_input.turn;
            current_mode->velocity = {0,0};
        }

        current_mode->rotation = f_input.turn;
        current_mode->throttle = f_input.throttle;

        if(autopilot.state == DONE) {
            //autopilot.state = NONE;
            autopilot.enter_local_space.EmitSignal();
            if(autopilot.landed) {
                autopilot.state = NONE;
                autopilot.landing_at_target.EmitSignal();
            }
            else {
                current_mode->velocity = {0,0};
                current_mode->throttle = 0.0f;
                AutopilotTarget dummy;
                ToggleAutoPilot(dummy);
            }
        }
    }
    else if(!g_input.in_use) {
        ManualFlightInput(dt);
    }
    FlightUpdate(position, dt);
}

void ShipController::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    Vector2 forward = {cosf(current_mode->rotation) * 100.0f, sinf(current_mode->rotation) * 100.0f};

    forward = Vector2Add(screen, forward);
    
    //DrawCircleV(screen, ship_data->radius * scale, PINK);
    DrawLineV(screen, forward, RED);

    if(autopilot_on) {
        DrawLineV(screen, Vector2Add(screen, Vector2Scale(autopilot.to_target, 100.0f)), ORANGE);
        DrawLineV(screen, Vector2Add(screen, Vector2Scale(current_mode->velocity, 0.1f)), GREEN);
        DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.target_velocity, 0.1f)), 5, BLUE);
        DrawCircleV(Vector2Add(screen, Vector2Scale(autopilot.to_target, current_mode->throttle * 100.0f)), 5, MAGENTA);

    }

    //printf("SHIP DRAW  %0.3f  %0.3f  %0.3f\n", screen.x, screen.y, ship_data->radius);

}


bool ShipController::ToggleAutoPilot(AutopilotTarget &target) {
    
    
    autopilot.SetTarget(target);

    if(autopilot_on){
        autopilot_on = false;
        return autopilot_on;
    }
    //printf("autopilot %i\n", autopilot_on);

    if(!target.set) {
        autopilot.target_data = {};
        autopilot_on = false;
    }
    else {
        autopilot_on = true;
    }

    return autopilot_on;

}

bool ShipController::ToggleFlightAssist() {
    flight_assist_on = !flight_assist_on;
    return flight_assist_on;
}

void ShipController::SetFlightMode(FLIGHT_MODE mode) {
    flight_mode = mode;
    
    flight_modes[flight_mode].velocity = current_mode->velocity;
    flight_modes[flight_mode].rotation = current_mode->rotation;
    flight_modes[flight_mode].throttle = current_mode->throttle;

    current_mode = &flight_modes.at(flight_mode);
    autopilot.flight_mode = current_mode;
}

void ShipController::AutopilotUpdate(Vector2 position) {
    //autopilot.Update();
}

void ShipController::FlightAssistUpdateUpdate(Vector2 &position) {

}

void ShipController::ManualFlightInput(float dt) {
/*     if(g_input.in_use) {
        return;
    } */
    

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


void ShipController::FlightUpdate(Vector2 &position, float dt) {


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

void ShipController::SetFlightModes() {

    printf("set fm   equimnent tags: %i\n", ship_data->equipment_tags.size());

    SystemDriveData &drive = g_system_drive_data[ (SHIP_PART_ID)ship_data->equipment_tags[EQUIPMENT_SYSTEM_DRIVE].part_id ];

    flight_modes[SYSTEM_FLIGHT_MODE].thrust = drive.thrust;
    flight_modes[SYSTEM_FLIGHT_MODE].reverse_thrust = drive.reverse_thrust;
    flight_modes[SYSTEM_FLIGHT_MODE].turn_speed = drive.turn_speed;
    flight_modes[SYSTEM_FLIGHT_MODE].drag = drive.drag;
    flight_modes[SYSTEM_FLIGHT_MODE].max_speed = drive.max_speed;

    flight_modes[LOCAL_FLIGHT_MODE].thrust = drive.thrust * 0.25f;
    flight_modes[LOCAL_FLIGHT_MODE].reverse_thrust = drive.reverse_thrust * 0.25f;
    flight_modes[LOCAL_FLIGHT_MODE].turn_speed = drive.turn_speed;
    flight_modes[LOCAL_FLIGHT_MODE].drag = drive.drag * 4;
    flight_modes[LOCAL_FLIGHT_MODE].max_speed = drive.max_speed * 0.10;
}