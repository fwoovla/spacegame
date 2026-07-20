#pragma once

#include "entity.hpp"
//#include "../scenes/scenes.hpp"
#include "system.hpp"
#include <vector>
#include "../FastNoisLite.h"

struct Chunk {
    Vector2i chunk_pos;
    std::vector<BaseEntity *> entity_list;
};



struct UniverseData {
    uint64_t seed;
    int max_systems = 2;
    std::unordered_map<int, Vector2> ui_map_positions;

    
    
    //StarSystemData system_data;
};


class UniverseManager {
    public:
        UniverseManager(){};
        ~UniverseManager(){};
        void CreateUniverse(std::string player_name);
        void UpdateBodies(Vector2 target_pos);
        void UpdateEntities();

        void Draw();
        void DrawUI();

    UniverseData universe_data;
    std::unique_ptr<System> current_system;

};

struct LocationData {
    float radius = 0.0f;
    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<BaseEntity>> entity_list;

    std::unique_ptr<TransitionSite> launch_site;
};

class LocationManager {
    public:
        LocationManager();
        ~LocationManager();
        void CreateLocation();
        void Update(Vector2 player_position);

        void Draw();
        void DrawUI();

        void OnLaunchRequested();


    LocationData location_data;
    Signal launch_requested;

};