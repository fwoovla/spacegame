#include "autppilot.hpp"
#include "string"
#include "../../utils/utils.hpp"
#include <raymath.h>
//#include "flightconrol.hpp"

Autopilot::Autopilot() {

}
Autopilot::~Autopilot() {

}
FlightInput Autopilot::Update(const AutopilotInput &input, const float dt) {
    if(target_data.set) {
        switch (state)
        {
        case ALIGN:
            return Align(input, dt);
            break;
        case ACCELERATE:
            return Accelerate(input, dt);
            break;
        case CRUISE:
            return Cruise(input, dt);
            break;
        case BRAKE:
            return Brake(input, dt);
            break;
        case ARRIVE:
            return Arrive(input, dt);
            break;
        
        default:
            /* what are you doing here? */
            break;
        }
        //printf("doing autopilot stuff  position:  %0.2f %0.2f    rotation:  %0.2f    velocity:  %0.2f  %0.2f\n", input.position.x, input.position.y, input.rotation, input.velocity.x, input.velocity.y);
    }

    
    FlightInput output;
    return output;

}

void Autopilot::SetTarget(AutopilotTarget target) {

    target_data = target;
    state = ALIGN;
    printf("  autopilot target %0.2f %0.2f, set: %i\n", target_data.position.x, target_data.position.y, target.set);

}

void Autopilot::ClearTarget() {
    target_data = {};

}

void Autopilot::EngageAutopilot() {

}



FlightInput Autopilot::Align(const AutopilotInput &input, const float dt) {
    FlightInput new_input;


    new_input.throttle = 0.0f;

    float target_angle = GetAngleFromTo(input.position, target_data.position);
    new_input.turn =  RotateTowardsRad(input.rotation, target_angle, 1.0f, dt );
    float diff = new_input.turn - target_angle;
    printf("turn - target angle %0.4f", diff);
    if(diff > -0.00001f and diff <0.00001f) {
        state = ACCELERATE;
    }
    printf("Align\n");
    return new_input;
}

FlightInput Autopilot::Accelerate(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    
    
    float distance = Vector2Distance(input.position, target_data.position);

    Vector2 to_target = Vector2Normalize(target_data.position - input.position);

    float closing_speed = Vector2DotProduct(input.velocity, to_target);
    float deceleration = flight_mode->reverse_thrust;

    float stopping_distance = closing_speed * closing_speed / (2.0f * deceleration);

    if (closing_speed > 0.0f && distance <= stopping_distance) {
        state = BRAKE;
    }

    float target_angle = GetAngleFromTo(input.position, target_data.position);
    new_input.turn =  RotateTowardsRad(input.rotation, target_angle, 1.0f, dt );

    //new_input.turn =  input.rotation;
    new_input.throttle = 1.0f;
    printf("Accelerate\n");
    return new_input;
}

FlightInput Autopilot::Cruise(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    
    printf("Cruise\n");
    return new_input;
}

FlightInput Autopilot::Brake(const AutopilotInput &input, const float dt) {
    FlightInput new_input;

    new_input.throttle = -0.5f;

    //float target_angle = GetAngleFromTo(input.position, target_data.position);
    //new_input.turn =  RotateTowardsRad(input.rotation, target_angle, 1.0f, dt );
    //new_input.turn = input.rotation;

    Vector2 to_target = Vector2Normalize(target_data.position - input.position);

    float closing_speed = Vector2DotProduct(input.velocity, to_target);

    if (closing_speed <= 0.0f) {
    // We've stopped moving toward the target
        state = ARRIVE;
        new_input.throttle = 0.0f;
        flight_mode->velocity = {0,0};
    }

    printf("Brake\n");
    return new_input;
}

FlightInput Autopilot::Arrive(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    new_input.turn = input.rotation;
    new_input.throttle = 0.0f;
    
    printf("Arrive\n");
    return new_input;
}