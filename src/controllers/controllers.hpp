#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../uilayers/flightcontrol/autppilot.hpp"




//++++++++++++++++++++++++++++++++++++++++++++++ SHIP ++++++++++++++++++++++++++++++++++++
enum SHIP_ID {
    SHIP_NONE = -1,
    SHIP_1,
};



//this is thie ship
//a ship is:
//a visual representation,
//a controller including all flight systems
//a collection of components

struct ShipTemplateData {  //initial ship data.  
    SHIP_ID id = SHIP_NONE;    // ship class/type
    std::string name = "no name";
    int value = 0;
    float radius = 0.0f;

};

extern std::unordered_map<int, ShipTemplateData> g_ship_template_data;

struct TargetData {
    int uid = -1;
    std::string name = "no name";
    float distance = 0.0f;
};


/* struct ShipControllerData {
    int uid;
    SHIP_ID id;
    std::string name = "no name";

    int value = 0;

    

    float radius = 0.0f;
  
};


 */

struct ShipData {
    int uid = -1;              

    SHIP_ID id = SHIP_NONE;    // ship class/type
    std::string name = "no name";

    int value = 0;
    float radius = 0.0f;

    int fuel = 0;

    //std::vector<ShipComponentData> components;
    //std::vector<ShipEquipmentData> equipment;
};






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

        ShipData *ship_data;

        FLIGHT_MODE flight_mode = SYSTEM_FLIGHT_MODE;

        std::array<FlightMode, 2> flight_modes;
        FlightMode *current_mode = nullptr;
        
        Autopilot autopilot;

        bool autopilot_on = false;
        bool flight_assist_on = false;
};



SHIP_ID StrToShipId(std::string str);



//++++++++++++++++++++++++++++++++++++++++++++++ CHARACTER ++++++++++++++++++++++++++++++++++++


enum CHARACTER_ID {
    CHARACTER_NONE = -1,
    CHARACTER_PLAYER,
};



struct CharacterMovement {
    float speed = 0.0f;
    Vector2 velocity = {0,0};
    float rotation = 0.0f;
};



struct CharacterControllerTemplateData {
    std::string name = "no name";
    CHARACTER_ID id = CHARACTER_NONE;
    int credits = 0;
    
    CharacterMovement movement;

    float radius = 0.0f;
};

extern std::unordered_map<int, CharacterControllerTemplateData> g_character_controller_template_data;


struct CharacterControllerData {
    int uid;
    std::string name = "no name";
    CHARACTER_ID id = CHARACTER_NONE;

    CharacterMovement movement;

    float radius = 0.0f;
};


class CharacterController {
    
    public:
        CharacterController(CharacterControllerData *_data);
        ~CharacterController();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);

        CharacterControllerData *character_data;
};


CHARACTER_ID StrToCharacterControllerId(std::string str);

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
