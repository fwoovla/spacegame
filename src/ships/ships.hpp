#pragma once 
#include <raylib.h>
#include "equipmenttemplates.hpp"
#include "../resources/resources.h"
#include "../controllers/controllers.hpp"




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

    std::vector<ShipEquipmentTag> equipment_tags;

};

extern std::unordered_map<int, ShipTemplateData> g_ship_template_data;



struct ShipData {
    int uid = -1;              

    SHIP_ID id = SHIP_NONE;    // ship class/type
    std::string name = "no name";

    int value = 0;
    float radius = 0.0f;

    int fuel = 0;

    std::vector<ShipEquipmentTag> equipment_tags;
    
};

SHIP_ID StrToShipId(std::string str);



class Ship {
    public:
    Ship(ShipData *_data);
    ~Ship() = default;

    void Update(Vector2 &position);
    void Draw(Vector2 &position, float scale);
    void Draw(Vector2 &position);
    void UseFuel(int fuel_use);

    ShipData *ship_data = nullptr;
    std::unique_ptr<ShipController> ship_controller;
};