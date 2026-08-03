#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../universe/components/components.hpp"

enum SHIP_ID {
    SHIP_NONE = -1,
    SHIP_1,
};




struct ShipMovement {
    Vector2 velocity = {0,0};
    float rotation = 0.0f;          // radians
    float thrust = 600.0f;          // acceleration
    float reverse_thrust = 300.0f;
    float max_speed = 3000.0f;
    float turn_speed = DEG2RAD * 180.0f;
    float drag = 0.1f;             // gameplay drag
    float throttle = 0.0f;          // 0-1
};






struct ShipTemplateData {
    SHIP_ID id;
    std::string name = "no name";

    int value = 0;

    ShipMovement movement;

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

    TargetData target_data;
    ShipMovement movement;

};


class Ship {
    
    public:
        Ship(ShipData _data);
        ~Ship();
        void Update(Vector2 &position);
        void Draw();

        ShipData ship_data;
};



SHIP_ID StrToShipId(std::string str);