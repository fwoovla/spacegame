#pragma once

#include "../uilayers/flightcontrol/flightcontrol.hpp"
#include "../uilayers/characterui/characterui.hpp"
#include "../uilayers/shopui/shopui.hpp"
#include "system.hpp"
#include "location.hpp"
#include <vector>
#include "../FastNoisLite.h"
#include "distributions.hpp"
#include "galaxystructures.hpp"

/* struct Chunk {
    Vector2i chunk_pos;
    std::vector<BaseEntity *> entity_list;
};

 */


 
struct SystemConnection {
    int uid = -1;
    int system_a_uid = -1;
    int system_b_uid = -1;

    bool discovered = false;
    bool activated = false;
};


struct GalaxyRegion; //see galaxygen

struct UniverseData {
    uint64_t seed;
    int max_systems = 0;
    float radius = 0.0f;

    std::vector<DistributionField> distributions;
    std::vector<GalaxyStructure> structures;
    std::vector<GalaxyRegion> galaxy_regions;

    std::unordered_map<int, SystemMapData> map_data;
    std::vector<SystemConnection> connections;

    std::unordered_map<int, ShipData> ship_data;
    std::unordered_map<int, CharacterData> character_data;

};


class UniverseManager {
    public:
        UniverseManager(){};
        ~UniverseManager(){};
        void CreateUniverse(std::string player_name);

        void OutlineUniverse();

        void GenerateDistributions();

        void ConnectSystems();

        void DiscoverSystemConnections(int system_uid);

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

        void OnOpenShop();
        void OnCloseShop();
        

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
    CharacterUI character_ui;

    bool shop_open = false;
    ShopUI shop_ui;

    FastNoiseLite system_noise;
};


EntityData GenerateEntityInstance(EntityTemplateData &tmpl, Vector2 position);

//system gen

void SystemGen_MakeSystem(SystemMapData &sys_map_data);
void SystemGen_GeneratePlanet(SystemBodyData &body_data);
void SystemGen_GenerateMoon(SystemBodyData &body_data);
void SystemGen_MakeLocations(SystemMapData &sys_map_data);
void SystemGen_MakeSites(SystemMapData &sys_map_data);

DistributionField GenerateDistribution(const float universe_radius, const DistributionSourceData &data);

SystemLocalData GenerateSystemLocalData(DistributionResult &distributions, GalaxyRegion &region);
SystemEnvironmentData GenerateSystemEnvironmentData(const GalaxyEnvironmentResult &environment);

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

// end system gen


//universe/galaxy gen

enum GALAXY_REGION_TYPE {
    GALAXY_REGION_CORE,
    GALAXY_REGION_BULDGE,
    GALAXY_REGION_DISC,
    GALAXY_REGION_ARM,
    GALAXY_REGION_COUNT
};

struct GalaxyRegion {
    GALAXY_REGION_TYPE region_type = GALAXY_REGION_CORE;

    float density = 0.0f;
    float radius = 0.0f;

    std::vector<Circle> circles;

};


void UniverseGen_MakeGalaxy(UniverseData &universe_data);

GalaxyRegion UniverseGen_GenerateRegion(GALAXY_REGION_TYPE type, float galaxy_radius);
GALAXY_REGION_TYPE GetGalaxyRegion(const std::vector<GalaxyRegion> &regions, Vector2 position);

// end universe/galaxy gen
