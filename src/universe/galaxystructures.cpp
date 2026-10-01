#include "galaxystructures.hpp"


std::vector<GalaxyStructureSourceData> galaxy_structure_data;


float GalaxyStructure::Sample(Vector2 p) const{

    float result = 0.0f;

    //for(const auto &source : sources) {
        if(CheckCollisionPointCircle( p, position, radius)) {
            float dist = Vector2Distance(p, position);
            float s_result = (1 - (dist / radius)) * strength;
            result += s_result;
        }
    //}

    return result;
}

GALAXY_STRUCTURE_TYPE StrToStructureType(const std::string& s) {
    static const std::unordered_map<std::string, GALAXY_STRUCTURE_TYPE> lookup_table = {
        {"GALAXY_STRUCTURE_GAS_CLOUD",                       GALAXY_STRUCTURE_TYPE::GALAXY_STRUCTURE_GAS_CLOUD},   
        {"GALAXY_STRUCTURE_DUST_CLOUD",                       GALAXY_STRUCTURE_TYPE::GALAXY_STRUCTURE_DUST_CLOUD}, 
        {"GALAXY_STRUCTURE_RADIATION_CLOUD",                       GALAXY_STRUCTURE_TYPE::GALAXY_STRUCTURE_RADIATION_CLOUD},
        {"GALAXY_STRUCTURE_VOID",                       GALAXY_STRUCTURE_TYPE::GALAXY_STRUCTURE_VOID},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "GALAXY_STRUCTURE_TYPE ID not found ");
    return GALAXY_STRUCTURE_TYPE::GALAXY_STRUCTURE_VOID;
}


GalaxyEnvironmentResult GetGalaxyEnvironmentResult(const std::vector<GalaxyStructure> &structures, Vector2 position) {

    GalaxyEnvironmentResult result;
    result.results.resize(GALAXY_STRUCTURE_COUNT);

    for(auto &structure : structures) {
        float s_value = structure.Sample(position);
        result.results[structure.type] += s_value;
    }

    return result;
}