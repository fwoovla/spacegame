#include "ships.hpp"


SHIP_ID StrToShipId(std::string str) {
    static const std::unordered_map<std::string, SHIP_ID> lookup_table = {
        {"SHIP_1",                       SHIP_ID::SHIP_1},   
    };

    if (auto it = lookup_table.find(str); it != lookup_table.end()) {
        TraceLog(LOG_INFO, "ship ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "Ship ID not found ");
    return SHIP_ID::SHIP_NONE  ;

}


SHIP_EQUIPMENT_ID StrToShipEquipmentId(const std::string& str) {

    static const std::unordered_map<std::string, SHIP_EQUIPMENT_ID> lookup_table = {
        {"EQUIPMENT_SYSTEM_DRIVE",     SHIP_EQUIPMENT_ID::EQUIPMENT_SYSTEM_DRIVE},
    };

    if (auto it = lookup_table.find(str); it != lookup_table.end()) {
        return it->second;
    }
    return SHIP_EQUIPMENT_ID::EQUIPMENT_SYSTEM_DRIVE;

}


SHIP_PART_ID StrToShipPartId(const std::string& str) {


    static const std::unordered_map<std::string, SHIP_PART_ID> lookup_table = {
        {"SYSTEM_DRIVE_MK1",     SHIP_PART_ID::SYSTEM_DRIVE_MK1},
        {"SYSTEM_DRIVE_MK2",     SHIP_PART_ID::SYSTEM_DRIVE_MK2},
    };

    if (auto it = lookup_table.find(str); it != lookup_table.end()) {
        return it->second;
    }
    return SHIP_PART_ID::SHIP_PART_NONE;

}