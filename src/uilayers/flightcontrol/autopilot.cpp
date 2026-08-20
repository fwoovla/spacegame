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
    if(target_data.set and flight_mode != nullptr) {

        distance = Vector2Distance(input.position, target_data.position);
        deceleration = flight_mode->reverse_thrust;

        flight_mode->speed = Vector2Length(flight_mode->velocity);

        to_target = Vector2Normalize(target_data.position - input.position);

        closing_speed = Vector2DotProduct(input.velocity, to_target);

        float max_arrival_speed = sqrtf(2.0f * deceleration * distance);
        float desired_speed = fminf(flight_mode->max_speed, max_arrival_speed);
        //float desired_speed = flight_mode->max_speed; //fmaxf(closing_speed, 0.0f);

        target_velocity = Vector2Scale(to_target, desired_speed); 
        velocity_error = Vector2Subtract(target_velocity, input.velocity);

        float forward_speed = Vector2DotProduct(input.velocity, to_target);
        Vector2 forward_velocity = Vector2Scale(to_target, forward_speed);

        lateral_velocity = Vector2Subtract(input.velocity, forward_velocity);
        
        stopping_distance = closing_speed * closing_speed / (2.0f * deceleration);
        //target_angle = GetAngleFromTo(input.position, target_data.position);
/* 
        printf("autopilot position:  %0.2f %0.2f    rotation:  %0.2f    velocity:  %0.2f  %0.2f  closing speed: %0.2f\n",
            input.position.x,
            input.position.y,
            input.rotation,
            input.velocity.x,
            input.velocity.y,
            closing_speed

        );
 */
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
        case LANDING:
            return Land(input, dt);
            break;
        
        default:
            /* what are you doing here? */
            break;
        }
        
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
    //printf("turn - target angle %0.4f", diff);
    if(diff > -0.00001f and diff <0.00001f) {
        state = ACCELERATE;
    }
    //printf("Align\n");
    return new_input;
}

FlightInput Autopilot::Accelerate(const AutopilotInput &input, const float dt) { //thanks AI
    FlightInput new_input;

    float error_length = Vector2Length(velocity_error);

    Vector2 thrust_direction = { 0, 0 };

    if(error_length > 0.01f) {
        thrust_direction = Vector2Normalize(velocity_error);
    }



    

    if(error_length > 0.01f){
        target_angle =
            GetAngleFromTo(
                {0, 0},
                thrust_direction
            );

        new_input.turn =
            RotateTowardsRad(
                input.rotation,
                target_angle,
                flight_mode->turn_speed,
                dt
            );
    }
    else{
        new_input.turn = input.rotation;
    }


    float forward_speed =
        Vector2DotProduct(input.velocity, thrust_direction);

    float desired_forward_speed =
        Vector2Length(target_velocity);

    float speed_error =
        desired_forward_speed - forward_speed;


    float throttle = speed_error / flight_mode->max_speed;
    new_input.throttle = Clamp(throttle, 0.0f, 1.0f);


    float lateral_speed = Vector2Length(lateral_velocity);
    float trajectory_error = lateral_speed / fmaxf(flight_mode->speed, 1.0f);

    if(distance <= stopping_distance && trajectory_error < 0.15f) {
        state = BRAKE;
    }
/* 
    if(distance <= stopping_distance){
        state = BRAKE;
    } */


    return new_input;
    
    //printf("Accelerate   speed: %0.0f,   stopping dist: %0.0f\n", speed, stopping_distance);
    //return new_input;
}

FlightInput Autopilot::Cruise(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    
    printf("Cruise\n");
    return new_input;
}

FlightInput Autopilot::Brake(const AutopilotInput &input, const float dt) {



    FlightInput new_input;

    Vector2 brake_direction = Vector2Normalize(input.velocity);

    brake_direction = Vector2Scale(brake_direction, 1.0f);

    float brake_angle = GetAngleFromTo({0, 0}, brake_direction);

    new_input.turn =
        RotateTowardsRad(
            input.rotation,
            brake_angle,
            flight_mode->turn_speed,
            dt
        );

    new_input.throttle = -0.5f;



/*     
    new_input.throttle = -0.5f;
    new_input.turn = input.rotation;
    Vector2 to_target = Vector2Normalize(target_data.position - input.position);
    float closing_speed = Vector2DotProduct(input.velocity, to_target);
    */
   
   if (closing_speed <= 1.0f) {
   // We've stopped moving toward the target
       state = ARRIVE;
       new_input.throttle = 0.0f;
       //flight_mode->velocity = {0,0};
   }

    //printf("Brake\n");
    return new_input;
}

FlightInput Autopilot::Arrive(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    new_input.turn = input.rotation;

    //new_input.throttle = 0.1f;
    
    printf("Arrive\n");
    if(target_data.auto_land == true) {
        printf("----LANDING\n");
        state = LANDING;
    }
    else {
        state = DONE;
    }

    return new_input;
}


FlightInput Autopilot::Land(const AutopilotInput &input, const float dt) {
    FlightInput new_input;
    new_input.turn = input.rotation;
    new_input.throttle = 0.0f;

    

    if( (input.position.x - target_data.position.x) < 10 and (input.position.y - target_data.position.y) < 10) {
        state = DONE;
        landed = true;
    }
    else {
        state = ALIGN;
    }


    printf("Landing\n");
    return new_input;
}

