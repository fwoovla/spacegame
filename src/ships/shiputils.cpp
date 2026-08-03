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

