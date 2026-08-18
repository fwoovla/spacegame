#include "location.hpp"
#include "../game.h"




Location::Location(LocationMapData &_map_data) : map_data(_map_data){

}

void Location::GenerateLocation(SelectionManager *sm) {

    selection_manager = sm;

    printf("generating system %i with  %i sites\n", map_data.uid, map_data.sites.size());

    for(auto &site : map_data.sites) {
        SpawnLocationSite(site.second);
    }

    for(auto &site : location_data.site_list) {
        site->RegisterWithManagers(selection_manager);
        
    }

}

void Location::Update() {
    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }

    std::erase_if(vec, [](const std::unique_ptr<CreatureEntity> &entity){return entity->should_delete;});


    location_data.ship->Update();

    if(g_input.keys_pressed[0] == KEY_SPACE and location_data.ship->can_board_ship) {
        launch_requested.EmitSignal();
    } 

}

void Location::Draw() {

}

void Location::DrawWorld() {

    DrawCircleV({0,0}, map_data.radius, DARKGRAY);

    for(auto &site : location_data.site_list) {
        site->Draw();
    }

    location_data.ship->Draw();

    for(auto &entity : location_data.entity_list) {
        if(entity->entity_data->render_mode == RENDER_WORLD)
            entity->Draw();       
    }
}

void Location::DrawOverlay() {

    
    for(auto &site : location_data.site_list) {
        site->DrawOverlay();
    }

    location_data.ship->DrawOverlay();

    for(auto &entity : location_data.entity_list) {
        if(entity->entity_data->render_mode != RENDER_WORLD)
            entity->DrawOverlay();
    }
}

void Location::DrawDebug() {

}

void Location::DrawUI() {
    for(auto &entity : location_data.entity_list) {
        entity->DrawUI();
    }
}


void Location::OnLaunchRequested() {

    launch_requested.EmitSignal();
}

PlayerCharacter * Location::SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData entity_data = GenerateEntityInstance(tmpl, uid, position);
    location_data.entity_data[entity_data.uid] = entity_data;

    std::unique_ptr<PlayerCharacter> player = std::make_unique<PlayerCharacter>(&location_data.entity_data[entity_data.uid]);
    PlayerCharacter * ptr = player.get();
    location_data.entity_list.push_back(std::move(player));

    return ptr;
}


LocationSite * Location::SpawnLocationSite(LocationSiteData &data) {
    std::unique_ptr<LocationSite> site = std::make_unique<LocationSite>(&data);
    LocationSite * ptr = site.get();

    location_data.site_list.push_back(std::move(site));
    printf("spawning site   |uid: %i   |location uid: %i\n", ptr->site_data->uid, ptr->site_data->location_uid);
    return ptr;
}



void Location::AddPlayer() {

    Vector2 player_pos = {0,0};
    for(auto & site : map_data.sites) {
        if(site.second.uid == g_game_data.transition.site_id) {
            player_pos = site.second.position;
        }
    }
    g_current_player->entity_data->position = player_pos;

    location_data.ship = std::make_unique<LocationShip>(&g_current_player->ship_data, player_pos);

    location_data.ship->RegisterWithManagers(selection_manager);
}


