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

    hud.SetTarget(g_current_player, current_system.get(), &selection_manager);
    //hud.selection_manager = &selection_manager;
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

        SystemBodyData star_body_data = GenerateSystemStarData(new_map_data);
        new_map_data.bodies[star_body_data.uid] = star_body_data;

        int p_tally = 0;
        for(int o_layer = 5; o_layer < star_body_data.orbital_layer_count; o_layer++) {

            if(GetRandomValue(0,100) > 75 and p_tally < star_body_data.orbital_body_count) {
                                
                SystemBodyData body_data = GenerateSystemBodyData(BODY_PLANET, 
                    o_layer, 
                    star_body_data.orbital_layer_delta, 
                    &star_body_data);

                p_tally++;
                new_map_data.bodies[body_data.uid] = body_data;
                star_body_data.orbital_body_uids.push_back(body_data.uid);
            }

        }

        std::vector<SystemBodyData*> planets;

        for (auto &[uid, body] : new_map_data.bodies) {
            if (body.body_type == BODY_PLANET)
                planets.push_back(&body);
        }


        for (SystemBodyData *planet : planets) {
            int m_tally = 0;
            
            for(int o_layer = 5; o_layer < planet->orbital_layer_count; o_layer++) {
                if(GetRandomValue(0,100) > 75 and m_tally < planet->orbital_body_count) {               
                    SystemBodyData moon_data = GenerateSystemBodyData(BODY_MOON, o_layer, planet->orbital_layer_delta, planet);
                    new_map_data.bodies[moon_data.uid] = moon_data;
                    planet->orbital_body_uids.push_back(moon_data.uid);
                    m_tally++;
                }
            }
        }



        GenerateLocations(new_map_data);

        GenerateSites(new_map_data);


        //PopulateSystem(new_map_data);
        
        universe_data.map_data[system_uid] = new_map_data;
        printf("new system:            uid %i           \"%s\"   layer delta = %0.5f\n", 
            new_map_data.uid, 
            new_map_data.name.c_str(),
            star_body_data.orbital_layer_delta 
        );
    }
    printf("\n\nEND OUTLINING UNIVERSE\n\n");
}




void UniverseManager::GenerateLocations(SystemMapData &map_data) {
    std::vector<SystemBodyData*> bodies;

    for (auto &[uid, body] : map_data.bodies) {
            if (body.landable == true)
                bodies.push_back(&body);
                //printf("location added --- name: %s\n", body.name.c_str());
        }

    int s = 1;
    for (SystemBodyData *body : bodies) {

        SystemLocationData new_location = GenerateSystemLocationData(body);
        map_data.locations[new_location.uid] = new_location;

    }

}

void UniverseManager::GenerateSites(SystemMapData &map_data) {

    std::vector<SystemLocationData*> locations;

    for (auto &[uid, location] : map_data.locations) {
        locations.push_back(&location);
        //printf("locations added name: %s\n", body.name.c_str());
    }
    for (SystemLocationData *location : locations) {
        for(int s = 0; s < location->local_data.site_amount; s++) {
            SystemSiteData new_site = GenerateSystemSiteData(location);
            map_data.sites[new_site.uid] = new_site;
            location->site_uids.push_back(new_site.uid);
        }
    }
}



void UniverseManager::GenerateNewSystem(int system_uid) {
    printf("generating  system     %i\n", system_uid);

    current_system = std::make_unique<System>(universe_data.map_data[system_uid]);

    current_system->GenerateSystem(&selection_manager);
    current_system->landing_requested.Connect( [&]() { OnLandAtLocationRequested();});
}





void UniverseManager::Update() {

    if(location_ready_to_destroy) {
        location_ready_to_destroy = false;
        LaunchFromLocation();
        current_location.reset();
        return;
    }
    if(location_ready_to_load) {
        location_ready_to_load = false;
        LandAtLocation();
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
    
    if(g_current_player->ship) {
        hud.Update();
    }
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
                SystemBody &body = *current_system->system_data.body_list[b];
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

    if(g_current_player->ship) {
        hud.Draw();
    }
}



void UniverseManager::OnLandAtLocationRequested() {
    if(location_active) {
        return;
    }
    location_ready_to_load = true;
    //LandAtLocation();
}



void UniverseManager::LandAtLocation() {


    if(!location_active) {
        location_active = true;

        g_game_data.transition.return_position = g_current_player->entity_data->position;

        selection_manager.UnregisterAll();

        LocationMapData new_location_map = GenerateLocationMapData(current_system.get(), g_game_data.transition.location_id);

        current_location = std::make_unique<Location>(new_location_map);
        current_location->GenerateLocation(&selection_manager);

        

        int uid = g_current_player->entity_data->uid; 
       
        //get data
        auto &data_to_move = current_system->system_data.entity_data[uid];
        //add to location
        current_location->location_data.entity_data[uid] = std::move(data_to_move);
        //erase from system
        current_system->system_data.entity_data.erase(uid);

        auto &fresh_data = current_location->location_data.entity_data[uid];

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
        //restore data to player
        g_current_player->entity_data = &fresh_data;

        current_location->launch_requested.Connect([this]() { LaunchFromLocationRequested();});

        g_current_player->ExitShip();
        exit_ship.EmitSignal();
        current_location->AddPlayer();

        g_camera.target = g_current_player->entity_data->position;
        g_camera.zoom = 1.0f;

        printf("transition to: %i position: %0.5f %0.5f\n", g_game_data.transition.location_id, g_current_player->entity_data->position.x, g_current_player->entity_data->position.y);
    }
}



void UniverseManager::LaunchFromLocationRequested() {
    if(!location_active) {
        return;
    }
    location_ready_to_destroy = true;
    //LaunchFromLocation();
}


void UniverseManager::LaunchFromLocation() {
    
    if(!location_active)
        return;

    int uid = g_current_player->entity_data->uid;

    //Vector2 return_position = g_game_data.transition.return_position;

    selection_manager.UnregisterAll();
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


    g_current_player = dynamic_cast<PlayerCharacter*>(system_entities.back().get());

    g_current_player->entity_data = &current_system->system_data.entity_data[uid];

    g_current_player->entity_data->position = g_game_data.transition.return_position;


    // Destroy location
    if(save_location){ /*save here*/ };

    location_active = false;


    // Reset camera
    g_camera.target = g_current_player->entity_data->position;


    g_current_player->EnterShip();

    enter_ship.EmitSignal();

    printf("returned to system %f %f\n",
        g_current_player->entity_data->position.x,
        g_current_player->entity_data->position.y);
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
    instance_data.parent_uid = uid;


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
    instance_data.parent_uid = parent->uid;

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

    printf("body data created  %0.5f %0.5f  delta %0.5f\n", instance_data.position.x, instance_data.position.y + layer, instance_data.orbital_layer_delta);
    return instance_data;
}



SystemLocationData GenerateSystemLocationData(SystemBodyData *body) {

    SystemLocationData data;
    data.uid = GetUID();
    data.body_uid = body->uid;
    data.system_uid = body->system_uid;

    data.name = "location " + std::to_string(data.uid) + " on body " + std::to_string(data.body_uid);

    data.radius = 50;
    data.position = body->position;

    Vector2 location_pos = {0,0};
    float l_angle = GetRandomValue(0, 360) * DEG2RAD;
    location_pos.x = data.position.x + cosf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius) );
    location_pos.y = data.position.y + sinf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius));
    data.position = location_pos;


    data.local_data.uid = data.uid;
    data.local_data.name = data.name;
    data.local_data.site_amount = 2;//GetRandomValue(0,2);
    data.local_data.location_plan = GenerateNewPlan();

    printf("location data created   name: %s\n", data.name.c_str());
    return data;
}

SystemSiteData GenerateSystemSiteData(SystemLocationData *location) {

    SystemSiteData new_site;

    new_site.uid = GetUID();
    new_site.location_uid = location->uid;
    new_site.body_uid = location->body_uid;
    new_site.system_uid = location->system_uid;
    new_site.name = "site" +   std::to_string(new_site.uid) +  " @ location " + std::to_string(location->uid);


    new_site.radius = 10;
    new_site.position = location->position;

    Vector2 site_pos = {0,0};
    float s_angle = GetRandomValue(0, 360) * DEG2RAD;
    site_pos.x = new_site.position.x + cosf(s_angle) * (location->radius - GetRandomValue(0, (int)location->radius));
    site_pos.y = new_site.position.y + sinf(s_angle) * (location->radius - GetRandomValue(0, (int)location->radius));
    new_site.position = site_pos;

    new_site.local_data.uid = new_site.uid;
    new_site.local_data.name = new_site.name;

    printf("site data created    name: %s  uid: %i \n", new_site.name.c_str(), new_site.uid);
    return new_site;


}

LocationMapData GenerateLocationMapData(System *system, int location_uid) {

    SystemLocationData &sys_map_data = system->map_data.locations[location_uid];


    LocationMapData new_location;

    new_location.uid = sys_map_data.uid;
    new_location.body_uid = sys_map_data.body_uid;
    new_location.system_uid = sys_map_data.system_uid;
    new_location.radius = sys_map_data.radius * 10;
    new_location.local_data = &sys_map_data.local_data;
    new_location.name = sys_map_data.name;
    

    for(int &site_uid : sys_map_data.site_uids) {

        SystemSiteData &s_site_data = system->map_data.sites[site_uid];

        LocationSiteData new_site = GenerateLocationSiteData(&s_site_data);

        new_location.sites[new_site.uid] = new_site;
    }
    new_location.sites;

    return new_location;
    
}

LocationSiteData GenerateLocationSiteData(SystemSiteData *s_site) {
    LocationSiteData new_site;
    
    new_site.uid = s_site->uid;
    new_site.location_uid = s_site->location_uid;
    new_site.body_uid = s_site->body_uid;
    new_site.system_uid = s_site->system_uid;
    new_site.name = s_site->name;


    new_site.radius = s_site->radius * 10;
    new_site.position = {0,0};

    new_site.local_data = &s_site->local_data;

    printf("site data created    name: %s  uid: %i \n", new_site.name.c_str(), new_site.uid);

    return new_site;
    
}