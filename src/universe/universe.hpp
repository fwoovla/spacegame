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
    int max_systems = 0;

    std::unordered_map<int, SystemMapData> map_data;

};


class UniverseManager {
    public:
        UniverseManager(){};
        ~UniverseManager(){};
        void CreateUniverse(std::string player_name);
        void OutlineUniverse();
        void GenerateNewSystem(int system_uid);

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



//PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
//void Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position);
EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);



//SystemBodyEntity * SpawnSystemBody(Vector2 position, BODY_TYPE type, int num_bodies, SystemBodyEntity *parent_body);

SystemBodyData GenerateSystemBodyData(BODY_TYPE type, int num_bodies);