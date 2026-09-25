#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "string"
#include <unordered_map>

enum DISTRIBUTION_TYPE {
    DISTRIBUTION_POPULATION,
    DISTRIBUTION_TECHNOLOGY,
    DISTRIBUTION_INDUSTRY,
    DISTRIBUTION_FACTION,
    DISTRIBUTION_COUNT
};


DISTRIBUTION_TYPE StrToDistributionType(const std::string& s);

struct DistributionSource {
    DISTRIBUTION_TYPE type = DISTRIBUTION_POPULATION;
    Vector2 position;
    float radius;
    float strength;
};

struct DistributionField {
     DISTRIBUTION_TYPE type = DISTRIBUTION_POPULATION;
    //float base_value = 0.0f;
    std::vector<DistributionSource> sources;
    float Sample(Vector2 position) const;
};

struct DistributionResult {
    std::vector<float> results;
};


DistributionResult GetDistributionResult(const std::vector<DistributionField> &distributions, Vector2 position);

struct DistributionSourceData {
    DISTRIBUTION_TYPE type = DISTRIBUTION_POPULATION;
    int source_count = 0;
    float max_radius_scale = 0.0f;
    float min_radius_scale = 0.0f;
    float strength_scale = 0.0f;

};

extern std::vector<DistributionSourceData> distribution_data;
