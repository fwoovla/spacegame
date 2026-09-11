#pragma once

#include "../uilayers/flightcontrol/flightcontrol.hpp"
#include "system.hpp"
#include "location.hpp"
#include <vector>
#include "../FastNoisLite.h"

struct Chunk {
    Vector2i chunk_pos;
    std::vector<BaseEntity *> entity_list;
};



struct SystemConnection {
    int uid = -1;
    int system_a_uid = -1;
    int system_b_uid = -1;

    bool discovered = false;
    bool activated = false;
};


struct UniverseData {
    uint64_t seed;
    int max_systems = 0;
    float radius = 0.0f;

    std::unordered_map<int, SystemMapData> map_data;
    std::vector<SystemConnection> connections;
    std::unordered_map<int, ShipData> ship_data;

};


class UniverseManager {
    public:
        UniverseManager(){};
        ~UniverseManager(){};
        void CreateUniverse(std::string player_name);
        void OutlineUniverse();
        void ConnectSystems();
        void DiscoverSystemConnections(int system_uid);

        //void GenerateLocations(SystemMapData &map_data);
        //void GenerateSites(SystemMapData &map_data);

        //void PopulateSystem(SystemMapData &map_data);
        void GenerateNewSystem(int system_uid);

        void Update();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();


        void OnTravelToSystemRequested();
        void TravelToSystem();

        void OnLandAtLocationRequested();
        void LandAtLocation();

        void LaunchFromLocationRequested();
        void LaunchFromLocation();

        int SelectRandomSystem();
        

    SelectionManager selection_manager;
    UniverseData universe_data;

    std::unique_ptr<System> current_system;
    std::unique_ptr<Location> current_location;

    bool location_active = false;
    bool save_system = false;
    bool save_location = false;


    bool location_ready_to_load = false;
    bool location_ready_to_destroy = false;

    bool system_ready_to_load = false;
    bool system_ready_to_destroy = false;

    Signal enter_ship;
    Signal exit_ship;

    FlightControl hud;

    FastNoiseLite system_noise;
};


void UniverseGen_MakeSystem(SystemMapData &sys_map_data);
void GeneratePlanet(SystemBodyData &body_data);
void GenerateMoon(SystemBodyData &body_data);
void UniverseGen_MakeLocations(SystemMapData &sys_map_data);
void UniverseGen_MakeSites(SystemMapData &sys_map_data);


EntityData GenerateEntityInstance(EntityTemplateData &tmpl, Vector2 position);

SystemBodyData GenerateSystemStarData(SystemMapData &map_data);

SystemBodyData GenerateSystemBodyData(BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent);

SystemLocationData GenerateSystemLocationData(SystemBodyData *parent);

SystemSiteData GenerateSystemSiteData(SystemLocationData *parent, int uid);

LocationMapData GenerateLocationMapData(System *system, int location_uid);

LocationSiteData GenerateLocationSiteData(SystemSiteData *site, Vector2 position);



std::string StarTypeToStr(STAR_TYPE star_type);
std::string CompositionTypeToStr(BODY_COMPOSITION composition_type);
std::string EnvoronmentTypeToStr(BODY_ENVIRONMENT environment_type);
std::vector<Color> GetBodyColors(SystemBodyData &body_data);
