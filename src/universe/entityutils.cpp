#include "entity.hpp"
#include "../scenes/scenes.hpp"
#include "../game.h"



int GetUID() {
    return GetRandomValue(100, 10000000);
}



ENTITY_ID StrToEntityId(const std::string& s) {

    static const std::unordered_map<std::string, ENTITY_ID> lookup_table = {
        {"ENTITY_PLAYER",                       ENTITY_ID::ENTITY_PLAYER},   
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        TraceLog(LOG_INFO, "entity ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "Entity ID not found ");
    return ENTITY_ID::ENTITY_NONE;
}
