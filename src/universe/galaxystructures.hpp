#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "string"
#include <unordered_map>

enum GALAXY_STRUCTURE_TYPE {
    GALAXY_STRUCTURE_GAS_CLOUD,
    GALAXY_STRUCTURE_DUST_CLOUD,
    GALAXY_STRUCTURE_RADIATION_CLOUD,
    GALAXY_STRUCTURE_VOID,
    GALAXY_STRUCTURE_COUNT
};


GALAXY_STRUCTURE_TYPE StrToStructureType(const std::string& s);

struct GalaxyStructure {
    GALAXY_STRUCTURE_TYPE type = GALAXY_STRUCTURE_VOID;
    Vector2 position;
    float radius;
    float strength;
    float Sample(Vector2 p) const;
};



struct GalaxyEnvironmentResult {
    std::vector<float> results;
};


GalaxyEnvironmentResult GetGalaxyEnvironmentResult(const std::vector<GalaxyStructure> &structures, Vector2 position);

struct GalaxyStructureSourceData {
    GALAXY_STRUCTURE_TYPE type = GALAXY_STRUCTURE_VOID;
    float gas = 0.0f;
    float dust = 0.0f;
    float radiation = 0.0f;
    float habitability = 0.0f;

};

extern std::vector<GalaxyStructureSourceData> galaxy_structure_data;
