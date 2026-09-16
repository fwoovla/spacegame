#include "universe.hpp"
#include "../game.h"
#include <algorithm>
#include <cmath>
#include <random>


void UniverseManager::CreateUniverse(std::string player_name) {
    printf("creating a  new universe for player:   %s\n", player_name.c_str());

    universe_data.max_systems = 500;
    universe_data.radius = 500000.0f;
    universe_data.seed = GetRandomValue(0, 100000);

    OutlineUniverse();

    for(auto &[uid, system] : universe_data.map_data) {
        UniverseGen_MakeSystem(system);
    }

    
    ConnectSystems();

    int selected_system = SelectRandomSystem();

    GenerateNewSystem(selected_system);
    current_system->map_data.discovered = true;

    ShipTemplateData ship_Tdata = g_ship_template_data[SHIP_1];

    ShipData ship_data;

    ship_data.id = ship_Tdata.id;
    ship_data.name = ship_Tdata.name;
    ship_data.uid = GetUID();
    ship_data.value = ship_Tdata.value;
    ship_data.radius = ship_Tdata.radius;
    ship_data.max_fuel = ship_Tdata.max_fuel;
    
    ship_data.fuel = ship_data.max_fuel;

    ship_data.equipment_tags = ship_Tdata.equipment_tags;

    printf("ship tags\n");
    for(auto &tag : ship_data.equipment_tags) {
        printf("tag  e id: %i  p id: %i\n",tag.equipment_id, tag.part_id);
    }

    universe_data.ship_data[ship_data.uid] = ship_data;


    CharacterTemplateData character_Tdata = g_character_template_data[CHARACTER_PLAYER];
    CharacterData character_data;
    character_data.id = character_Tdata.id;
    character_data.name = character_Tdata.name;
    character_data.radius = character_Tdata.radius;
    character_data.speed = character_Tdata.speed;
    character_data.uid = GetUID();

    universe_data.character_data[character_data.uid] = character_data;


    g_current_player = current_system->SpawnNewPlayer(g_entity_template_data[ENTITY_PLAYER], 0, current_system->system_data.star_position);
    g_current_player->entity_data->ship_uid = ship_data.uid;
    g_current_player->entity_data->character_uid = character_data.uid;
    g_current_player->EnterShip(&universe_data.ship_data[g_current_player->entity_data->ship_uid]);

    if(g_current_player != nullptr) {
    }
    else {
        printf("could not find player\n");
    }

    hud.SetTarget(g_current_player, current_system.get(), &selection_manager, &universe_data);
    //hud.selection_manager = &selection_manager;
}

//create the SystemMapData
void UniverseManager::OutlineUniverse() {
    printf("\n\nOUTLINING UNIVERSE\n\n");



    //initial system data
    for(int system_index = 0 ; system_index < universe_data.max_systems; system_index++) {
        int system_uid = GetUID();
        
        SystemMapData new_map_data;
        
        new_map_data.uid = system_uid;
        new_map_data.name = "system " + std::to_string(system_uid);
        new_map_data.radius = 100000.0f;
        
        new_map_data.star_position = {new_map_data.radius, new_map_data.radius};

        new_map_data.map_position = { 
            (float)GetRandomValue(-(int)universe_data.radius, (int)universe_data.radius), 
            (float)GetRandomValue(-(int)universe_data.radius, (int)universe_data.radius) 
        };
        universe_data.map_data[system_uid] = new_map_data;
    }
}


void UniverseManager::ConnectSystems() {

    //=================================================  SETUP ==========================================================================
    struct GraphEdge {
        int a;
        int b;
        float distance;
        bool selected = false;
    };

    int extra_connections = 25;

    universe_data.connections.clear();
    int num_systems = universe_data.map_data.size();

    if(num_systems < 2) {
         return;
    }

    std::vector<GraphEdge> edges;

    std::vector<SystemMapData *> map_data_vector;
    for(auto &system : universe_data.map_data) {
        map_data_vector.push_back(&system.second);
    }

    for(int i = 0; i < num_systems; ++i) {
        for(int j = i+1; j < num_systems; j++) {
            GraphEdge edge;
            edge.a = i;
            edge.b = j;

            edge.distance = Vector2DistanceSqr(map_data_vector[i]->map_position, map_data_vector[j]->map_position);
            edges.push_back(edge);
        }
    }


    std::sort(
        edges.begin(),
        edges.end(),
        [](const GraphEdge& a, const GraphEdge& b)
        {
            return a.distance < b.distance;
        }
    );

    std::vector<int> parent(num_systems);

    for(int i = 0; i < num_systems; ++i)
        parent[i] = i;

    auto Find = [&](auto&& self, int x) -> int
    {
        if(parent[x] == x)
            return x;

        parent[x] = self(self, parent[x]);
        return parent[x];
    };

    auto Union = [&](int a, int b)
    {
        a = Find(Find, a);
        b = Find(Find, b);

        if(a == b)
            return false;

        parent[b] = a;
        return true;
    };

    //=================================================  END SETUP ==========================================================================

    // ------------------------------------------------------------
    // First pass:
    //
    // Build a minimum spanning tree.
    //
    // This guarantees every system is reachable.
    // ------------------------------------------------------------


    int connection_uid = 0;

    for(auto& edge : edges)
    {
        if(Union(edge.a, edge.b))
        {
            edge.selected = true;
            SystemConnection connection;

            connection.uid = connection_uid++;
            connection.system_a_uid = map_data_vector[edge.a]->uid;
            connection.system_b_uid = map_data_vector[edge.b]->uid;
            connection.discovered = false;
            connection.activated = false;
/*             
            if(GetRandomValue(0, 100) > 50) {
                connection.activated = true;
            }
            else {
                connection.activated = false;
            }
 */
            universe_data.connections.push_back(connection);
            //printf("first pass connection: %i  from: %i   to: %i\n", connection.uid, connection.system_a_uid, connection.system_b_uid);

            if(static_cast<int>(universe_data.connections.size()) == num_systems - 1)
                break;
        }
    }

    // ------------------------------------------------------------
    // Second pass: SAVE THIS FOR LATER.  JUST USING MST FOR NOW
    //
    // Add extra edges.
    //
    // These create loops in the graph.
    // ------------------------------------------------------------

/* 
    std::vector<GraphEdge> possible_extra_edges;

    for(const auto& edge : edges) {
        if(!edge.selected) {
            possible_extra_edges.push_back(edge);
        }
    }

    std::mt19937 rng;

    std::shuffle(
        possible_extra_edges.begin(),
        possible_extra_edges.end(),
        rng
    );
    

    for(const auto& edge : possible_extra_edges)
    {
        //printf("extra new edge %i %i\n", edge.a, edge.b);
        if(extra_connections <= 0)
            break;

        SystemConnection connection;

        connection.uid = connection_uid++;
        connection.system_a_uid = map_data_vector[edge.a]->uid;
        connection.system_b_uid = map_data_vector[edge.b]->uid;

        universe_data.connections.push_back(connection);
        printf("seccond pass connection: %i  from: %i   to: %i\n", connection.uid, connection.system_a_uid, connection.system_b_uid);

        --extra_connections;
    }
 */
    printf("\ntotal connections: %i\n", universe_data.connections.size());
/*     for(auto &connection : universe_data.connections) {
        printf("connection: %i  from: %i   to: %i\n", connection.uid, connection.system_a_uid, connection.system_b_uid);
    } */

}


void UniverseManager::DiscoverSystemConnections(int system_uid) {
    for(auto &connection : universe_data.connections)
    {
        if(connection.system_a_uid == system_uid ||
           connection.system_b_uid == system_uid)
        {
            connection.discovered = true;
            connection.activated = true;

            universe_data.map_data[connection.system_a_uid].examined = true;
            universe_data.map_data[connection.system_b_uid].examined = true;
        }
    }
}


void UniverseManager::GenerateNewSystem(int system_uid) {
    current_system.reset();
    printf("generating  system     %i\n", system_uid);

    current_system = std::make_unique<System>(universe_data.map_data[system_uid]);

    universe_data.map_data[system_uid].discovered = true;

    current_system->GenerateSystem(&selection_manager);
    DiscoverSystemConnections(current_system->system_data.uid);

    current_system->landing_requested.Connect( [&]() { OnLandAtLocationRequested();});
    current_system->system_travel_requested.Connect( [&]() { OnTravelToSystemRequested();});
}



void UniverseManager::Update() {
    if(current_system == nullptr) { return;}

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

    if(system_ready_to_load) {
        system_ready_to_load = false;
        TravelToSystem();
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



void UniverseManager::OnTravelToSystemRequested() {
    if(!universe_data.map_data.contains(g_game_data.transition.system_id) or g_game_data.transition.system_id == current_system->system_data.uid) {
        return;
    }

    if(g_current_player->ship->ship_data->fuel <= 0) {
        printf("!!! out of fuel  !!!!\n");
        return;
    }

    g_current_player->ship->UseFuel(1);

    system_ready_to_load = true;
}

void UniverseManager::TravelToSystem() {

    selection_manager.UnregisterAll();

    int player_uid = g_current_player->entity_data->uid;

    //get data
    EntityData data_to_move = current_system->system_data.entity_data[player_uid];

    current_system->system_data.entity_data.clear();

    int destination_system = g_game_data.transition.system_id;
    GenerateNewSystem(destination_system);


    g_current_player = current_system->SpawnPlayer(data_to_move, current_system->system_data.star_position);
    g_current_player->EnterShip(&universe_data.ship_data[g_current_player->entity_data->ship_uid]);

    if(g_current_player != nullptr) {
    }
    else {
        printf("could not spawn player\n");
    }

    hud.SetTarget(g_current_player, current_system.get(), &selection_manager, &universe_data);

    printf("arived at new system!!!!\n--\n%s\n", current_system->system_data.name.c_str());
}


void UniverseManager::OnLandAtLocationRequested() {
    if(location_active) {
        return;
    }
    location_ready_to_load = true;
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

        auto &system_entities = current_system->system_data.creature_entity_list;

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

        g_current_player->ExitShip(&universe_data.character_data[g_current_player->entity_data->character_uid]);
        exit_ship.EmitSignal();
        current_location->AddPlayer();
        g_current_player->entity_data->render_mode = RENDER_WORLD;

        g_camera.target = g_current_player->entity_data->position;
        g_game_data.do_camera_transition = false;
        g_camera.zoom = 2.0f;

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

    int player_uid = g_current_player->entity_data->uid;

    selection_manager.UnregisterAll();
    //hud.ClearTarget();

    // Move entity data back to the system
    current_system->RegisterWithManagers();
    current_system->system_data.entity_data[player_uid] = std::move(current_location->location_data.entity_data[player_uid]);

    current_location->location_data.entity_data.erase(player_uid);


    // Move player entity ownership back
    auto &location_entities = current_location->location_data.entity_list;

    auto &system_entities = current_system->system_data.creature_entity_list;


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

    g_current_player->entity_data = &current_system->system_data.entity_data[player_uid];

    g_current_player->entity_data->position = g_game_data.transition.return_position;

    // Destroy location
    if(save_location){ /*save here*/ };

    location_active = false;

    // Reset camera
    g_camera.target = g_current_player->entity_data->position;
    g_camera.zoom = 10.5f;

    g_current_player->EnterShip(&universe_data.ship_data[g_current_player->entity_data->ship_uid]);

    enter_ship.EmitSignal();
    g_current_player->entity_data->render_mode = RENDER_CAPPED;

    printf("returned to system %f %f\n",
        g_current_player->entity_data->position.x,
        g_current_player->entity_data->position.y);
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
EntityData GenerateEntityInstance(EntityTemplateData &tmpl, Vector2 position) {

    EntityData instance_data;

    instance_data.uid = GetUID();
    instance_data.id = tmpl.id;
    instance_data.name = tmpl.name;
    instance_data.id = tmpl.id;
    instance_data.obstructable = tmpl.obstructable;
    instance_data.position = position;
    instance_data.render_mode = tmpl.render_mode;

    //instance_data.ship_uid = tmpl.ship_id;
    //instance_data.character_controller_id = tmpl.character_controller_id;
    instance_data.object_entity_controller_id = tmpl.object_entity_controller_id;
    
    return instance_data;
}





