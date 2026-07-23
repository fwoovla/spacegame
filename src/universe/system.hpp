#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"






struct SystemInstanceData {
    std::string name = "system";
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
        void GenerateSystem();

        void Update();
        void Draw();
        void DrawUI();

        void GenerateBodies();
        void GenerateLandingSites();
        void OnTransitionClicked();


        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        void Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position);
        EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);



        SystemBodyEntity * SpawnSystemBody(Vector2 position, BODY_TYPE type, int orbitals, SystemBodyEntity *parent_body);
        SystemBodyData GenerateSystemBodyInstance(Vector2 position, BODY_TYPE type, int orbitals);

        SystemInstanceData system_data;

        Signal landing_requested;
};

















