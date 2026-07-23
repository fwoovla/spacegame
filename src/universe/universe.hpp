#pragma once

#include "entity.hpp"
//#include "../scenes/scenes.hpp"
#include "system.hpp"
#include "location.hpp"
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

        void Update();
        void Draw();
        void DrawUI();

        void OnLandAtLocationRequested();
        void LandAtLocation();
        void LaunchFromLocationRequested();
        void LaunchFromLocation();
        void TravelToSystemRequested();
        void TravelToSystem();

       
    UniverseData universe_data;

    std::unique_ptr<System> current_system;
    std::unique_ptr<Location> current_location;

    bool location_active = false;
    bool save_system = false;
    bool save_location = false;

    bool location_ready_to_destroy = false;
    bool system_ready_to_destroy = false;
};


/* 
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

}; */