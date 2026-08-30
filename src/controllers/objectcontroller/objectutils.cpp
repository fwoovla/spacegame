#include "../controllers.hpp"

OBJECTENTITY_ID StrToObjectEntityControllerId(std::string str) {
    static const std::unordered_map<std::string, OBJECTENTITY_ID> lookup_table = {
        {"OBJECTENTITY_1",                       OBJECTENTITY_ID::OBJECTENTITY_1},   
    };

    if (auto it = lookup_table.find(str); it != lookup_table.end()) {
        TraceLog(LOG_INFO, "object entitiy ID found %i", it->second);
        return it->second;
    }
    TraceLog(LOG_INFO, "object entitiy ID not found ");
    return OBJECTENTITY_ID::OBJECTENTITY_NONE ;

}
