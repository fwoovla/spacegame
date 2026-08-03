#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"
#include"location.hpp"
#include "../input/selectionmanager.hpp"



struct SystemMapData {
    int uid = -1;
    std::string name = "system name";
    Vector2 map_position;

    float radius = 0.0f;

    Vector2 star_position;

    int orbital_layer_count = 0;
    float orbital_layer_delta = 0.0f;
    int orbital_body_count = 0;

    std::unordered_map<int, LocationMapData> location_data;

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
        void GenerateSystem(SystemMapData &map_data, SelectionManager *sm);

        void Update();
        void Draw();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        void OnTransitionClicked();


        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        SystemBodyEntity * SpawnSystemBody(SystemBodyData &data);

        void ResolveBodyParents();

        SelectionManager *selection_manager = nullptr;

        SystemInstanceData system_data;

        Signal landing_requested;
};

















