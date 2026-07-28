#include "universe.hpp"
#include "../game.h"


void UniverseManager::CreateUniverse(std::string player_name) {
    printf("creating a  new universe for player:   %s\n", player_name.c_str());

    universe_data.max_systems = 1;

    OutlineUniverse();

    int s = 0;
    int num_systems = universe_data.map_data.size();
    int system_uids[num_systems];
    for(auto &system: universe_data.map_data) {
        system_uids[s] = system.second.uid;
        s++;
    }


    int rand_index = GetRandomValue(0, num_systems-1);


    GenerateNewSystem(system_uids[rand_index]);

    g_current_player = current_system->SpawnPlayer(g_entity_template_data[ENTITY_PLAYER], 0, current_system->system_data.star_position); 

    if(g_current_player != nullptr) {
    }
    else {
        printf("could not find system\n");
    }
}

//create the SystemMapData
void UniverseManager::OutlineUniverse() {
    printf("\n\nOUTLINING UNIVERSE\n\n");
    for(int system_index = 0 ; system_index < universe_data.max_systems; system_index++) {
        int system_uid = GetUID();
        
        SystemMapData new_data;
        
        new_data.uid = system_uid;
        new_data.name = "system " + std::to_string(system_uid);
        new_data.radius = 50000.0f;
        
        new_data.star_position = {new_data.radius, new_data.radius};

        

        new_data.map_position = {0,0};

        new_data.orbital_layer_count = GetRandomValue(5, 10);
        new_data.orbital_layer_delta = new_data.radius/new_data.orbital_layer_count;

        new_data.orbital_body_count = 5;


        SystemBodyData star_data = GenerateSystemBodyData(BODY_STAR, 0, 0.0f, nullptr);
        star_data.position = new_data.star_position;
        new_data.body_data[star_data.uid] = star_data;


        //main bodies orbiting star
        int p_tally = 0;
        SystemBodyData *star = &current_system->system_data.body_data[0];

        for(int o_layer = 5; o_layer < new_data.orbital_layer_count; o_layer++) {

            if(GetRandomValue(0,100) > 0 and p_tally < new_data.orbital_body_count) {
                                
                SystemBodyData body_data = GenerateSystemBodyData(BODY_PLANET, o_layer, new_data.orbital_layer_delta, star);
                
                /* body_data.orbit_radius = o_layer * new_data.orbital_layer_delta;
                body_data.orbit_angle = DEG2RAD * GetRandomValue(0, 359);           

                Vector2 pos;

                pos.x = new_data.star_position.x + cosf(body_data.orbit_angle) * body_data.orbit_radius;
                pos.y = new_data.star_position.y + sinf(body_data.orbit_angle) * body_data.orbit_radius;

                body_data.position = pos; */

                p_tally++;
                
                new_data.body_data[body_data.uid] = body_data;
            }
        }



        universe_data.map_data[system_uid] = new_data;
        printf("new system:            uid %i           \"%s\"   layer delta = %0.5f\n", 
            new_data.uid, 
            new_data.name.c_str(),
            new_data.orbital_layer_delta );


    }
    printf("\n\nEND OUTLINING UNIVERSE\n\n");
}
















void UniverseManager::GenerateNewSystem(int system_uid) {
    printf("generating  system     %i\n", system_uid);

    current_system = std::make_unique<System>();
    current_system->GenerateSystem(universe_data.map_data[system_uid]);
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


/*  */
EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData instance_data;


    instance_data.uid = GetUID();
    instance_data.name = tmpl.name;
    instance_data.id = tmpl.id;
    instance_data.obstructable = tmpl.obstructable;
    instance_data.position = position;


    instance_data.component_flags = tmpl.component_flags;

    instance_data.health = tmpl.health;
    instance_data.inventory = tmpl.inventory;
    instance_data.movement = tmpl.movement;
    instance_data.interaction = tmpl.interaction;

    instance_data.collision_rect = {
        position.x,
        position.y,
        tmpl.size.x,
        tmpl.size.y
    };

    instance_data.radius = tmpl.size.x/2;

    return instance_data;
}


/*  */
SystemBodyData GenerateSystemBodyData( BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent) {

    SystemBodyData instance_data;
    instance_data.body_type = type;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.obstructable = false;
    instance_data.position = {0,0};  //set on return

    if(type == BODY_STAR) {
        instance_data.name = "star " + std::to_string(uid);
        instance_data.modulate = ORANGE;
        instance_data.radius = 1500.0f;
        instance_data.orbiting_bodies_count = GetRandomValue(0,8);
        instance_data.parent = nullptr;
    }
    else if(type == BODY_PLANET) {
        instance_data.name = "planet " + std::to_string(uid);
        instance_data.modulate = BLUE;
        instance_data.radius = 200.0f;
        instance_data.orbiting_bodies_count = GetRandomValue(0,4);
        instance_data.parent = parent;
    }
    else if(type == BODY_MOON) {
        instance_data.name = "moon " + std::to_string(uid);
        instance_data.modulate = YELLOW;
        instance_data.radius = 50.0f;
        instance_data.orbiting_bodies_count = 0;
        instance_data.parent = parent;

    }

    instance_data.orbit_radius = layer * layer_delta;
    instance_data.orbit_angle = DEG2RAD * GetRandomValue(0, 359);           

    Vector2 pos = {0,0};

    if(parent != nullptr) {
        pos.x = parent->position.x + cosf(instance_data.orbit_angle) * instance_data.orbit_radius;
        pos.y = parent->position.y + sinf(instance_data.orbit_angle) * instance_data.orbit_radius;
    }

    instance_data.position = pos;
    
    printf("body  %0.5f %0.5f\n", pos.x, pos.y + layer);
    return instance_data;
}