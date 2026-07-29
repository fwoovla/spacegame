#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"


struct SystemMapData {
    int uid = -1;
    std::string name = "system name";
    Vector2 map_position;

    float radius = 0.0f;

    Vector2 star_position;

    int orbital_layer_count = 0;
    float orbital_layer_delta = 0.0f;
    int orbital_body_count = 0;

    //std::vector<SystemBodyData> bodies;
    std::unordered_map<int, SystemBodyData> body_data;

};



struct SystemInstanceData {
    int uid = -1;
    std::string name = "system ";
    uint64_t seed;
    
    float radius;

    Vector2 star_position;  //multiple stars coming soon

    std::vector<float> orbitals; 

    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<BaseEntity>> entity_list;

    std::unordered_map<int, SystemBodyData> body_data;
    std::vector<std::unique_ptr<SystemBodyEntity>> body_list;
};



class System {
    public:
        void GenerateSystem(SystemMapData &map_data);

        void Update();
        void Draw();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        void GenerateBodies();
        void GenerateLandingSites();
        void OnTransitionClicked();


        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        SystemBodyEntity * SpawnSystemBody(SystemBodyData &data, SystemBodyEntity *parent_body);
        SystemBodyEntity * GenerateAndSpawnSystemBody(Vector2 position, BODY_TYPE type, int num_bodies, SystemBodyEntity *parent_body);
/*         
        void Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position);
        EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);




        SystemBodyData GenerateSystemBodyInstance(Vector2 position, BODY_TYPE type, int num_bodies); */

        SystemInstanceData system_data;

        Signal landing_requested;
};

















