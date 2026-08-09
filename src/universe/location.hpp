#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"





struct LocationInstanceData {
    std::string name = "location";
    uint64_t seed;
    int uid;
    float radius = 0.0f;
    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<BaseEntity>> entity_list;

    //std::vector<LandingSiteData> landing_sites;
};

class Location {
    public:
        void GenerateLocation(LocationMapData &map_data);

        void Update();
        void Draw();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        void OnLaunchRequested();

        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        void Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position);
        EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);

        LocationInstanceData location_data;

        std::vector<LandingSite> landing_sites;

        Signal launch_requested;
};

