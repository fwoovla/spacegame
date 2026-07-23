#include "universe.hpp"
#include "../game.h"


void UniverseManager::CreateUniverse(std::string player_name) {
    printf("creating a  new universe for player:   %s\n", player_name.c_str());

    universe_data.max_systems = 1;

    OutlineUniverse();

    GenerateNewSystem(universe_data.map_data[0].uid);

    if(g_current_player != nullptr) {
        g_current_player = current_system->SpawnPlayer(g_entity_template_data[ENTITY_PLAYER], 0, {current_system->system_data.radius - 500, current_system->system_data.radius}); 
    }
    else {
        printf("could not find system\n");
    }

}

//create the SystemMapData
void UniverseManager::OutlineUniverse() {
    for(int system_index = 0 ; system_index < universe_data.max_systems; system_index++) {
        int system_uid = GetUID();
        
        SystemMapData new_data;
        
        new_data.uid = system_uid;
        new_data.name = "system " + std::to_string(system_uid);
        
        universe_data.map_data[system_uid] = new_data;
        printf("new system %i  %s\n", new_data.uid, new_data.name.c_str());
    }

}


void UniverseManager::GenerateNewSystem(int system_uid) {

    current_system = std::make_unique<System>();

    //GenerateNewSystem(universe_data.map_data[0].uid);
    current_system->GenerateSystem();
    current_system->landing_requested.Connect( [&]() { OnLandAtLocationRequested();});

}


void UniverseManager::Update() {

    if(location_ready_to_destroy) {
        current_location.reset();
        location_ready_to_destroy = false;
        return;
    }


    switch(location_active)
    {
        case false:
            current_system->Update();
            break;
            
        case true:
            current_location->Update();
            break;

    }
}







void UniverseManager::Draw() {



    switch(location_active)
    {
        case false:
            current_system->Draw();
            break;
        
        case true:
            current_location->Draw();
            break;
    }

    

}


void UniverseManager::DrawUI() {

    switch(location_active)
    {
        case false:
            current_system->DrawUI();
            break;
        
        case true:
            current_location->DrawUI();
            break;
    }
}



void UniverseManager::OnLandAtLocationRequested() {
    if(location_active) {
        return;
    }
    LandAtLocation();

}



void UniverseManager::LandAtLocation() {


    if(!location_active) {
        location_active = true;

        current_location = std::make_unique<Location>();
        current_location->GenerateLocation();

        int uid = g_current_player->entity_data->uid; 
       
        auto &old_data = current_system->system_data.entity_data[uid];

        current_location->location_data.entity_data[uid] = std::move(old_data);

        auto &new_data = current_location->location_data.entity_data[uid];

        current_system->system_data.entity_data.erase(uid);


        auto &system_entities = current_system->system_data.entity_list;

        for(auto it = system_entities.begin(); it != system_entities.end(); ++it)
        {
            if(it->get() == g_current_player)
            {
                

                // move ownership
                current_location->location_data.entity_list.push_back(std::move(*it));
                system_entities.erase(it);
                break;
            }
        } 

        g_current_player = dynamic_cast<PlayerCharacter*>(current_location->location_data.entity_list.back().get());
        g_current_player->entity_data = &new_data;

        g_current_player->entity_data->position = {0,0};

        g_camera.target = g_current_player->entity_data->position;

        current_location->launch_requested.Connect([&]() { LaunchFromLocationRequested();});
        //location_scene = std::make_unique<LocationScene>();

        printf("transition activated %i  %0.5f %0.5f\n", g_game_data.transition.location_id, g_game_data.transition.return_position.x, g_game_data.transition.return_position.y);
    }
}



void UniverseManager::LaunchFromLocationRequested() {
    if(!location_active) {
        return;
    }
    LaunchFromLocation();
}


void UniverseManager::LaunchFromLocation() {
    
    if(!location_active)
        return;

    int uid = g_current_player->entity_data->uid;

    // Save return position before moving anything
    Vector2 return_position = g_game_data.transition.return_position;


    // Move entity data back to the system
    current_system->system_data.entity_data[uid] = std::move(current_location->location_data.entity_data[uid]);

    current_location->location_data.entity_data.erase(uid);


    // Move player entity ownership back
    auto &location_entities = current_location->location_data.entity_list;

    auto &system_entities = current_system->system_data.entity_list;


    for(auto it = location_entities.begin(); it != location_entities.end(); ++it)
    {
        if(it->get() == g_current_player)
        {
            system_entities.push_back(std::move(*it));
            location_entities.erase(it);
            break;
        }
    }


    // Re-acquire player pointer
    g_current_player = dynamic_cast<PlayerCharacter*>(system_entities.back().get());

    // Rebind entity data pointer
    g_current_player->entity_data = &current_system->system_data.entity_data[uid];


    // Restore position in system space
    g_current_player->entity_data->position = return_position;


    // Destroy location
    if(save_location){ /*save here*/ };
    location_active = false;


    // Reset camera
    g_camera.target = g_current_player->entity_data->position;


    printf("returned to system %f %f\n",
        return_position.x,
        return_position.y);
}



void UniverseManager::TravelToSystemRequested() {

}

void UniverseManager::TravelToSystem() {

}
