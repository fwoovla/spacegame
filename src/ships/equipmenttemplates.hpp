#pragma once 
#include <raylib.h>
#include "../resources/resources.h"

enum FUEL_TYPE {
    FUEL_CLASS_1,
    FUEL_CLASS_2,
};

struct FuelData {
    std::string name = "fuel";
    FUEL_TYPE fuel_type = FUEL_CLASS_1;
    float price = 0.0f;
};

extern std::unordered_map<FUEL_TYPE, FuelData> g_fuel_data;


enum SHIP_EQUIPMENT_ID {
    EQUIPMENT_NONE = -1,
    EQUIPMENT_SYSTEM_DRIVE = 0,
    EQUIPMENT_COUNT
};

enum SHIP_PART_ID {
    SHIP_PART_NONE = -1,
    SYSTEM_DRIVE_MK1,
    SYSTEM_DRIVE_MK2
};

SHIP_EQUIPMENT_ID StrToShipEquipmentId(const std::string& str);


struct ShipEquipmentTag {
    SHIP_EQUIPMENT_ID equipment_id = EQUIPMENT_NONE;
    int part_id = -1;
};


struct ShipEquipmentData {
    SHIP_EQUIPMENT_ID equipment_id = EQUIPMENT_NONE;
    SHIP_PART_ID part_id = SHIP_PART_NONE;
    std::string name = "no name";
    int value = 0;

    virtual ~ShipEquipmentData() = default;
};



struct SystemDriveData : ShipEquipmentData {
    float thrust = 0.0f;
    float reverse_thrust = 0.0f;
    float max_speed = 0.0f;
    float turn_speed = PI;
    float drag = 0.02f;
};


extern std::unordered_map<SHIP_PART_ID, SystemDriveData> g_system_drive_data;

extern std::unordered_map<SHIP_PART_ID, ShipEquipmentData*> g_ship_equipment_data;

SHIP_PART_ID StrToShipPartId(const std::string& str);



