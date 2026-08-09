#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"
#include"location.hpp"
#include "../input/selectionmanager.hpp"


/* struct SystemLandingSiteMapData {
    int uid = -1;
    int parent_uid = -1;
    std::string name = "";
    Vector2 position;
    //std::unique_ptr<TransitionArea> transition_area;
};
 */
struct SystemSiteLocalData {
    int uid = -1;
    std::string name = "no site name";
};


//===========================================================
/* struct SystemLocationMapData {
    int uid = -1;
    int body_uid = -1;
    int system_id = -1;
    std::string name = "location name";
    Vector2 position;

    float system_radius = 0.0f;
    float location_radius = 0.0f;

    std::vector<SystemLandingSiteMapData> landing_sites;

    //std::vector<SystemBodyData> bodies;
}; */

struct SystemLocationLocalData {
    int uid = -1;
    std::string name = "no location name";
};


//==========================================================
/* struct SystemBodyMapData {
    int uid = -1;
    int body_uid = -1;
    int system_id = -1;
    std::string name = "body name";
    Vector2 position;

    float location_radius = 0.0f;
    float system_radius = 0.0f;

    //std::vector<int> site_uids;

    //std::vector<SystemBodyData> bodies;
};
 */

struct SystemBodyLocalData {
    int uid = -1;
    std::string name = "no body name";
};

//==========================================================



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

    std::vector<std::unique_ptr<SystemBodyEntity>> body_list;
    std::vector<std::unique_ptr<SystemLocationEntity>> location_list;
    std::vector<std::unique_ptr<SystemSiteEntity>> site_list;


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
        SystemBodyEntity * SpawnSystemBody(SystemBodyData &data);
        SystemLocationEntity * SpawnSystemLocation(SystemLocationData &data);
        SystemSiteEntity * SpawnSystemSite(SystemSiteData &data);

        void ResolveParents();

        SelectionManager *selection_manager = nullptr;

        SystemInstanceData system_data;

        SystemMapData &map_data;

        Signal landing_requested;
};

















