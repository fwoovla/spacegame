#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"



struct LocationData {
    std::string name = "location";
    uint64_t seed;
    int uid;
    float radius = 0.0f;
    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<BaseEntity>> entity_list;

    std::unique_ptr<TransitionSite> launch_site;
};

class Location {
    public:
        void GenerateLocation();

        void Update();
        void Draw();
        void DrawUI();

        void OnLaunchRequested();

        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        void Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position);
        EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);

        LocationData location_data;

        Signal launch_requested;
};

