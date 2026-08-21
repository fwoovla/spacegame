#pragma once

#include "entity.hpp"
#include <vector>
#include "../FastNoisLite.h"
#include "locationobject/locationobject.hpp"
#include "../input/selectionmanager.hpp"
#include "../viewport/viewport.hpp"
#include "locationplan.hpp"





struct LocationMapData { //this is generated from SystemLocationMapData at runtime

    LocationLocalData *local_data;
    std::string name = "location name";
    int uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    float radius = 0.0f;

    std::unordered_map<int, LocationSiteData> sites;

};


struct LocationInstanceData {
    std::string name = "location";
    uint64_t seed;
    int uid;
    std::unordered_map<int, EntityData> entity_data;
    std::vector<std::unique_ptr<CreatureEntity>> entity_list;

    std::vector<std::unique_ptr<LocationSite>> site_list;
    std::unique_ptr<LocationShip> ship;
};


class Location {
    public:
        Location(LocationMapData &_map_data);
        void GenerateLocation(SelectionManager *sm);

        void Update();
        void Draw();
        void DrawWorld();
        void DrawOverlay();
        void DrawDebug();
        void DrawUI();

        void AddPlayer();

        void OnLaunchRequested();

        PlayerCharacter * SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position);
        LocationSite * SpawnLocationSite(LocationSiteData &data);

        LocationInstanceData location_data; 
        LocationMapData map_data; 
        SelectionManager *selection_manager = nullptr;

        //Signal landing_requested;
        Signal launch_requested;
};

