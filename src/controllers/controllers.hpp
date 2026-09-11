#pragma once 
#include <raylib.h>
#include "../characters/characters.hpp"
#include "../resources/resources.h"
#include "../uilayers/flightcontrol/autppilot.hpp"





//++++++++++++++++++++++++++++++++++++++++++++++ SHIP ++++++++++++++++++++++++++++++++++++

/* struct TargetData {
    int uid = -1;
    std::string name = "no name";
    float distance = 0.0f;
};


 */
class ShipData;
class ShipController {
    
    public:
        ShipController(ShipData *_data);
        ~ShipController();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);


        bool ToggleAutoPilot(AutopilotTarget &target);
        bool ToggleFlightAssist();
        void SetFlightMode(FLIGHT_MODE mode);

        void ManualFlightInput(float dt);
        void FlightUpdate(Vector2 &position, float dt);
        void AutopilotUpdate(Vector2 position);
        void FlightAssistUpdateUpdate(Vector2 &position);
        void SetFlightModes();

        ShipData *ship_data;

        FLIGHT_MODE flight_mode = SYSTEM_FLIGHT_MODE;

        std::array<FlightMode, 2> flight_modes;
        FlightMode *current_mode = nullptr;
        
        Autopilot autopilot;

        bool autopilot_on = false;
        bool flight_assist_on = false;
};







//++++++++++++++++++++++++++++++++++++++++++++++ CHARACTER ++++++++++++++++++++++++++++++++++++


struct CharacterMovement {
    float speed = 0.0f;
    Vector2 velocity = {0,0};
    float rotation = 0.0f;
};



class CharacterData;
class CharacterController {
    
    public:
        CharacterController(CharacterData *_data);
        ~CharacterController();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);

        CharacterData *character_data;
        CharacterMovement movement;
};




//++++++++++++++++++++++++++++++++++++++++++++++ OBJECT ++++++++++++++++++++++++++++++++++++


enum OBJECTENTITY_ID {
    OBJECTENTITY_NONE = -1,
    OBJECTENTITY_1,
};



struct ObjectEntityMovement {
    float speed = 0.0f;
    Vector2 velocity = {0,0};
    float rotation = 0.0f;
};



struct ObjectEntityControllerTemplateData {
    std::string name = "no name";
    OBJECTENTITY_ID id = OBJECTENTITY_NONE;
    int credits = 0;
    
    ObjectEntityMovement movement;

    float radius = 0.0f;
};

extern std::unordered_map<int, ObjectEntityControllerTemplateData> g_object_entity_controller_template_data;


struct ObjectEntityControllerData {
    int uid;
    std::string name = "no name";
    OBJECTENTITY_ID id = OBJECTENTITY_NONE;

    ObjectEntityMovement movement;

    float radius = 0.0f;
};


class ObjectEntityController {
    
    public:
        ObjectEntityController(ObjectEntityControllerData *_data);
        ~ObjectEntityController();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);

        ObjectEntityControllerData *object_controller_data;
};


OBJECTENTITY_ID StrToObjectEntityControllerId(std::string str);
