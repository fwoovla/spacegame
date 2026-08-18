#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"
#include "systemobject/systemobject.hpp"
//#include "location.hpp"
#include "../input/selectionmanager.hpp"
#include "../viewport/viewport.hpp"




struct SystemMapData {
    int uid = -1;
    std::string name = "system name";
    Vector2 map_position;

    float radius = 0.0f;

    Vector2 star_position;

    int orbital_layer_count = 0;
    float orbital_layer_delta = 0.0f;
    int orbital_body_count = 0;

    std::unordered_map<int, SystemBodyData> bodies;
    std::unordered_map<int, SystemLocationData> locations;
    std::unordered_map<int, SystemSiteData> sites;

};



struct SystemInstanceData {
    int uid = -1;
    std::string name = "system ";
    uint64_t seed;
    
    float radius;

    Vector2 star_position;  //multiple stars coming soon

    std::vector<float> orbitals; 

    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<CreatureEntity>> entity_list;

    std::vector<std::unique_ptr<SystemBody>> body_list;
    std::vector<std::unique_ptr<SystemLocation>> location_list;
    std::vector<std::unique_ptr<SystemSite>> site_list;


};



class System {
    public:
        System(SystemMapData &_map_data);
        void GenerateSystem(SelectionManager *sm);

        void Update();
        void Draw();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        SystemBody * SpawnSystemBody(SystemBodyData &data);
        SystemLocation * SpawnSystemLocation(SystemLocationData &data);
        SystemSite * SpawnSystemSite(SystemSiteData &data);

        void ResolveParents();

        void SetCameraState(CAMERA_STATE new_state);


        SelectionManager *selection_manager = nullptr;

        SystemInstanceData system_data;

        SystemMapData &map_data;

        Signal landing_requested;
};

















