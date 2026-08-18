#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../uilayers/flightcontrol/autppilot.hpp"


enum SHIP_ID {
    SHIP_NONE = -1,
    SHIP_1,
};




struct ShipTemplateData {
    SHIP_ID id;
    std::string name = "no name";

    int value = 0;
    float radius = 0.0f;

    FlightMode system_drive;
    FlightMode planet_drive;

};

extern std::unordered_map<int, ShipTemplateData> g_ship_template_data;

struct TargetData {
    int uid = -1;
    std::string name = "no name";
    float distance = 0.0f;
};


struct ShipData {
    int uid;
    SHIP_ID id;
    std::string name = "no name";

    int value = 0;

    FLIGHT_MODE flight_mode = SYSTEM_FLIGHT_MODE;

    TargetData target_data;

    std::array<FlightMode, 2> flight_modes; 

    float radius = 0.0f;
  
};

class Ship {
    
    public:
        Ship(ShipData *_data);
        ~Ship();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);


        bool ToggleAutoPilot(AutopilotTarget &target);
        bool ToggleFlightAssist();
        void SetFlightMode(FLIGHT_MODE mode);

        void ManualFlightInput(float dt);
        void FlightUpdate(Vector2 &position, float dt);
        void AutopilotUpdate(Vector2 position);
        void FlightAssistUpdateUpdate(Vector2 &position);

        ShipData *ship_data;
        FlightMode *current_mode = nullptr;
        
        Autopilot autopilot;

        bool autopilot_on = false;
        bool flight_assist_on = false;
};



SHIP_ID StrToShipId(std::string str);