#pragma once
#include <raylib.h>



enum FLIGHT_MODE {
    SYSTEM_FLIGHT_MODE,
    PLANET_FLIGHT_MODE
};



struct FlightMode {
    Vector2 velocity = {0,0};
    float rotation = 0.0f;          // radians
    float thrust = 600.0f;          // acceleration
    float reverse_thrust = 300.0f;
    float max_speed = 3000.0f;
    float turn_speed = DEG2RAD * 180.0f;
    float drag = 0.1f;             // gameplay drag
    float throttle = 0.0f;          // 0-1
    bool throttle_override = false;
};



enum AUTOPILOT_STATE {
    ALIGN,
    ACCELERATE,
    CRUISE,
    BRAKE,
    ARRIVE
};


struct FlightInput {
    float throttle = 0.0f;
    float turn = 0.0f;
};


struct AutopilotTarget {
    bool set = false;
    Vector2 position;
    bool auto_land = false;
};


struct AutopilotInput {
    Vector2 position;
    Vector2 velocity;
    float rotation;
};

class Autopilot {
    public:
        Autopilot();
        ~Autopilot();
        FlightInput Update(const AutopilotInput &input, float dt);

        void SetTarget(AutopilotTarget target);
        void ClearTarget();
        void EngageAutopilot();

        FlightInput Align(const AutopilotInput &input, const float dt);
        FlightInput Accelerate(const AutopilotInput &input, const float dt);
        FlightInput Cruise(const AutopilotInput &input, const float dt);
        FlightInput Brake(const AutopilotInput &input, const float dt);
        FlightInput Arrive(const AutopilotInput &input, const float dt);

        AutopilotTarget target_data;
        AUTOPILOT_STATE state;

        FlightMode *flight_mode;
};


