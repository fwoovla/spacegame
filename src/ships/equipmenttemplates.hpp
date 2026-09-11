#pragma once 
#include <raylib.h>
#include "../resources/resources.h"



enum SHIP_EQUIPMENT_ID {
    EQUIPMENT_SYSTEM_DRIVE = 0,
    EQUIPMENT_COUNT
};

SHIP_EQUIPMENT_ID StrToShipEquipmentId(const std::string& str);


struct ShipEquipmentTag {
    SHIP_EQUIPMENT_ID equipment_id;
    int part_id;
};


enum SHIP_PART_ID {
    SHIP_PART_NONE = 0,
    SYSTEM_DRIVE_MK1,
    SYSTEM_DRIVE_MK2
};

struct SystemDriveData{
    SHIP_EQUIPMENT_ID equipment_id = EQUIPMENT_SYSTEM_DRIVE;
    SHIP_PART_ID part_id = SHIP_PART_NONE;
    std::string name = "no name";
    int value = 0;

    float thrust = 100.0;
    float reverse_thrust = 50.0f;
    float max_speed = 1000.0f;
    float turn_speed = PI;
    float drag = 0.02;

};

extern std::unordered_map<SHIP_PART_ID, SystemDriveData> g_system_drive_data;

SHIP_PART_ID StrToShipPartId(const std::string& str);

