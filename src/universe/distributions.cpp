#include "distributions.hpp"

std::vector<DistributionSourceData> distribution_data;

float DistributionField::Sample(Vector2 position) const{

    float result = 0.0f;

    for(const auto &source : sources) {
        if(CheckCollisionPointCircle( position, source.position, source.radius)) {
            float dist = Vector2Distance(position, source.position);
            float s_result = (1 - (dist / source.radius)) * source.strength;
            result += s_result;
        }
    }

    return result;
}


DISTRIBUTION_TYPE StrToDistributionType(const std::string& s) {
    static const std::unordered_map<std::string, DISTRIBUTION_TYPE> lookup_table = {
        {"DISTRIBUTION_POPULATION",                       DISTRIBUTION_TYPE::DISTRIBUTION_POPULATION},   
        {"DISTRIBUTION_TECHNOLOGY",                       DISTRIBUTION_TYPE::DISTRIBUTION_TECHNOLOGY}, 
        {"DISTRIBUTION_INDUSTRY",                       DISTRIBUTION_TYPE::DISTRIBUTION_INDUSTRY},
        {"DISTRIBUTION_FACTION",                       DISTRIBUTION_TYPE::DISTRIBUTION_FACTION},
    };

    if (auto it = lookup_table.find(s); it != lookup_table.end()) {
        return it->second;
    }
    TraceLog(LOG_INFO, "Object Entity ID not found ");
    return DISTRIBUTION_TYPE::DISTRIBUTION_POPULATION;
}


DistributionResult GetDistributionResult(const std::vector<DistributionField> &distributions, Vector2 position) {

    DistributionResult result;
    result.results.resize(DISTRIBUTION_COUNT);

    for(auto &dist : distributions) {
        float s_value = dist.Sample(position);
        result.results[dist.type] += s_value;
    }

    return result;
}