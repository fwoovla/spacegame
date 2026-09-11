#include "characters.hpp"

CHARACTER_ID StrToCharacterId(std::string str) {
    static const std::unordered_map<std::string, CHARACTER_ID> lookup_table = {
        {"CHARACTER_PLAYER",                       CHARACTER_ID::CHARACTER_PLAYER},   
    };

    if (auto it = lookup_table.find(str); it != lookup_table.end()) {
        TraceLog(LOG_INFO, "character ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "character ID not found ");
    return CHARACTER_ID::CHARACTER_NONE  ;

}
