#include "universe.hpp"
#include "../game.h"


void UniverseManager::CreateUniverse(std::string player_name) {
    printf("creating a  new universe for player:   %s\n", player_name.c_str());

    universe_data.max_systems = 1;

    OutlineUniverse();

    int selected_system = SelectRandomSystem();

    GenerateNewSystem(selected_system);

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
        
        SystemMapData new_map_data;
        
        new_map_data.uid = system_uid;
        new_map_data.name = "system " + std::to_string(system_uid);
        new_map_data.radius = 100000.0f;
        
        new_map_data.star_position = {new_map_data.radius, new_map_data.radius};

        new_map_data.map_position = {0,0};

        SystemBodyData star_data = GenerateSystemStarData(new_map_data);

        new_map_data.body_data[star_data.uid] = star_data;

        //main bodies orbiting star
        int p_tally = 0;
        for(int o_layer = 5; o_layer < star_data.orbital_layer_count; o_layer++) {

            if(GetRandomValue(0,100) > 75 and p_tally < star_data.orbital_body_count) {
                                
                SystemBodyData body_data = GenerateSystemBodyData(BODY_PLANET, o_layer, star_data.orbital_layer_delta, &star_data);
                p_tally++;
                new_map_data.body_data[body_data.uid] = body_data;
            }
        }

        std::vector<SystemBodyData*> planets;

        for (auto &[uid, body] : new_map_data.body_data) {
            if (body.body_type == BODY_PLANET)
                planets.push_back(&body);
        }


        for (SystemBodyData *planet : planets) {
            if(planet->uid != star_data.uid) {

                int m_tally = 0;
                for(int o_layer = 5; o_layer < planet->orbital_layer_count; o_layer++) {

                    if(GetRandomValue(0,100) > 75 and m_tally < planet->orbital_body_count) {
                                        
                        SystemBodyData sat_data = GenerateSystemBodyData(BODY_MOON, o_layer, planet->orbital_layer_delta, planet);
                        new_map_data.body_data[sat_data.uid] = sat_data;
                        m_tally++;
                    }
                }

            }
        }

        PopulateSystem(new_map_data);
        
        universe_data.map_data[system_uid] = new_map_data;
        printf("new system:            uid %i           \"%s\"   layer delta = %0.5f\n", 
            new_map_data.uid, 
            new_map_data.name.c_str(),
            star_data.orbital_layer_delta 
        );
    }
    printf("\n\nEND OUTLINING UNIVERSE\n\n");
}








void UniverseManager::PopulateSystem(SystemMapData &map_data) {

    std::vector<SystemBodyData*> bodies;

    for (auto &[uid, body] : map_data.body_data) {
            if (body.landable == true)
                bodies.push_back(&body);
                printf("planet added name: %s\n", body.name.c_str());
        }

    int s = 1;
    for (SystemBodyData *body : bodies) {



        LocationMapData new_location;

        new_location.location_radius = 500;
        new_location.system_radius = 50;
        new_location.name = "station " + std::to_string(s);
        new_location.uid = GetUID();
        new_location.body_uid = body->uid;
        new_location.position = body->position;

        Vector2 location_pos = {0,0};
        float l_angle = GetRandomValue(0, 360) * DEG2RAD;
        location_pos.x = new_location.position.x + cosf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius) );
        location_pos.y = new_location.position.y + sinf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius));
        new_location.position = location_pos;



        LandingSiteData l_site;

        l_site.uid = GetUID();
        l_site.name = "landing pad @ station " + std::to_string(s);
        l_site.position = new_location.position;
        Vector2 site_pos = {0,0};
        float s_angle = GetRandomValue(0, 360) * DEG2RAD;
        site_pos.x = l_site.position.x + cosf(s_angle) * (new_location.system_radius - GetRandomValue(0, (int)new_location.system_radius));
        site_pos.y = l_site.position.y + sinf(s_angle) * (new_location.system_radius - GetRandomValue(0, (int)new_location.system_radius));
        l_site.position = site_pos;

        new_location.landing_sites.push_back(l_site);
        printf("new l_site     name: %s  uid: %i \n", l_site.name.c_str(), l_site.uid);

        printf("new location     name: %s  uid: %i \n", new_location.name.c_str(), new_location.uid);

        map_data.location_data[new_location.uid] = new_location;

        //body->landing_sites.push_back(l_site);
        s++;

        
    }

}







void UniverseManager::GenerateNewSystem(int system_uid) {
    printf("generating  system     %i\n", system_uid);

    current_system = std::make_unique<System>();
    current_system->GenerateSystem(universe_data.map_data[system_uid], &selection_manager);
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

    selection_manager.Update();
}







void UniverseManager::DrawWorld() {
    switch(location_active)
    {
        case false:
            current_system->DrawWorld();
            break;
        
        case true:
            current_location->DrawWorld();
            break;
    }
}


void UniverseManager::DrawOverlay() {
    switch(location_active)
    {
        case false:
            current_system->DrawOverlay();
            break;
        
        case true:
            current_location->DrawOverlay();
            break;
    }
}



void UniverseManager::DrawDebug() {
    switch(location_active)
    {
        case false:
            current_system->DrawDebug();
            for(int b = 0; b < current_system->system_data.body_list.size(); b++) {
                SystemBodyEntity &body = *current_system->system_data.body_list[b];
                DrawLineV(body.body_data->position, g_current_player->entity_data->position, GRAY);
            }
            break;
        
        case true:
            current_location->DrawDebug();
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

        LocationMapData &map_data = universe_data.map_data[current_system->system_data.uid].location_data[g_game_data.transition.location_id];

        current_location->GenerateLocation( map_data );
        
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
        g_camera.zoom = 1.0f;

        current_location->launch_requested.Connect([&]() { LaunchFromLocationRequested();});

        g_current_player->ExitShip();

        exit_ship.EmitSignal();

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


    g_current_player->EnterShip();

    enter_ship.EmitSignal();

    printf("returned to system %f %f\n",
        return_position.x,
        return_position.y);
}



void UniverseManager::TravelToSystemRequested() {

}

void UniverseManager::TravelToSystem() {

}



int UniverseManager::SelectRandomSystem() {

    int s = 0;
    int num_systems = universe_data.map_data.size();
    int system_uids[num_systems];
    for(auto &system: universe_data.map_data) {
        system_uids[s] = system.second.uid;
        s++;
    }

    int i = GetRandomValue(0, num_systems-1);
    return system_uids[i];
}






/*  */
EntityData GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData instance_data;

    instance_data.uid = GetUID();
    instance_data.name = tmpl.name;
    instance_data.id = tmpl.id;
    instance_data.obstructable = tmpl.obstructable;
    instance_data.position = position;
    instance_data.render_mode = tmpl.render_mode;

    return instance_data;
}




SystemBodyData GenerateSystemStarData(SystemMapData &map_data) {


    SystemBodyData instance_data;
    instance_data.body_type = BODY_STAR;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.obstructable = false;
    instance_data.landable = false;
    instance_data.position = map_data.star_position;
    instance_data.parent_body_uid = uid;


    instance_data.name = "star " + std::to_string(uid);
    instance_data.modulate = ORANGE;
    instance_data.radius = 6000.0f;

    instance_data.orbital_body_count = GetRandomValue(1,10);
    instance_data.orbital_layer_count = 20; //GetRandomValue(5, 20);
    instance_data.orbital_layer_delta = map_data.radius/instance_data.orbital_layer_count;

    
    printf("star  %0.5f %0.5f  delta %0.5f\n", instance_data.position.x, instance_data.position.y, instance_data.orbital_layer_delta);
    return instance_data;

}

/*  */
SystemBodyData GenerateSystemBodyData( BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent) {

    SystemBodyData instance_data;
    instance_data.body_type = type;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.obstructable = false;
    instance_data.landable = true;
    instance_data.position = {0,0};
    instance_data.parent_body_uid = parent->uid;

    if(type == BODY_PLANET) {
        instance_data.name = "planet " + std::to_string(uid);
        instance_data.modulate = BLUE;
        instance_data.radius = 2000.0f;
        instance_data.orbital_body_count = GetRandomValue(0,5);
        instance_data.orbital_layer_count = 20; //GetRandomValue(5, 20);
        instance_data.orbital_layer_delta = (instance_data.radius * 10) /instance_data.orbital_layer_count;
        
    }
    else if(type == BODY_MOON) {
        instance_data.name = "moon " + std::to_string(uid);
        instance_data.modulate = YELLOW;
        instance_data.radius = 800.0f;
        instance_data.orbital_body_count = 0;
    }

    instance_data.orbit_radius = layer * layer_delta;
    instance_data.orbit_angle = DEG2RAD * GetRandomValue(0, 359);           

    
    if(parent != nullptr) {

        Vector2 pos = {0,0};
        pos.x = parent->position.x + cosf(instance_data.orbit_angle) * instance_data.orbit_radius;
        pos.y = parent->position.y + sinf(instance_data.orbit_angle) * instance_data.orbit_radius;
        instance_data.position = pos;

        instance_data.parent_orbital = layer;
    }

    
    printf("body  %0.5f %0.5f  delta %0.5f\n", instance_data.position.x, instance_data.position.y + layer, instance_data.orbital_layer_delta);
    return instance_data;
}



