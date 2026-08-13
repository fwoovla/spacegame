#pragma once

//#include "entity.hpp"
//#include "../scenes/scenes.hpp"
//#include "../uilayers/uilayers.hpp"
#include "../uilayers/flightcontrol/flightcontrol.hpp"
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

        void GenerateLocations(SystemMapData &map_data);
        void GenerateSites(SystemMapData &map_data);

        //void PopulateSystem(SystemMapData &map_data);
        void GenerateNewSystem(int system_uid);

        void Update();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        void OnLandAtLocationRequested();
        void LandAtLocation();

        void LaunchFromLocationRequested();
        void LaunchFromLocation();

        void TravelToSystemRequested();
        void TravelToSystem();

        int SelectRandomSystem();
        

       
    UniverseData universe_data;

    std::unique_ptr<System> current_system;
    std::unique_ptr<Location> current_location;

    bool location_active = false;
    bool save_system = false;
    bool save_location = false;

    bool location_ready_to_destroy = false;
    bool system_ready_to_destroy = false;

    Signal enter_ship;
    Signal exit_ship;

    FlightControl hud;

    SelectionManager selection_manager;
};



EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position);

SystemBodyData GenerateSystemStarData(SystemMapData &map_data);

SystemBodyData GenerateSystemBodyData(BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent);

SystemLocationData GenerateSystemLocationData(SystemBodyData *parent);

SystemSiteData GenerateSystemSiteData(SystemLocationData *parent);

