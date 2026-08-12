#include "location.hpp"
#include "../game.h"



void Location::GenerateLocation(SystemLocationData &map_data) {


    location_data.uid = map_data.uid;

    location_data.radius = map_data.location_radius;
    location_data.seed = GetRandomValue(100, 100000);

    //for(auto &site : map_data.landing_sites) {

/*         LandingSite new_site;
        new_site.position = {0,0};
        new_site.name = site.name;
        new_site.uid = site.uid;
        new_site.info_area.size = {50, 50};
        new_site.info_area.position = {-20 ,-20};
        //new_site.info_area.type = LAUNCHING;
        new_site.info_area.mouse_triggered.Connect([&]() { OnLaunchRequested();});

        landing_sites.push_back(new_site); */
    //}

}


void Location::Update() {

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }

    std::erase_if(vec, [](const std::unique_ptr<CreatureEntity> &entity){return entity->should_delete;});

/*     for(auto &site : landing_sites) {
        //site.transition_area.Update();
    } */

    //location_data.launch_site->transition_area->Update();
}


void Location::Draw() {

    DrawCircleV({0,0}, location_data.radius, DARKGRAY);
    //DrawCircle( location_data.launch_site->position.x, location_data.launch_site->position.y, location_data.launch_site->transition_area->size.x, RED);

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Draw();
    }

}

void Location::DrawWorld() {

    DrawCircleV({0,0}, location_data.radius, DARKGRAY);
/* 
    for(auto &site : landing_sites) {
        DrawCircleV( site.position, site.info_area.radius, RED);
        //printf("landing site draw\n");

    }

    for(auto &entity : location_data.entity_list) {
        if(entity->entity_data->render_mode == RENDER_WORLD)
            entity->Draw();
    }
    */
}


void Location::DrawOverlay() {

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
    //location_data.launch_site->transition_area->Draw();
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


void Location::Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position) {


}

EntityData Location::GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData instance_data;


    instance_data.uid = GetUID();
    instance_data.name = tmpl.name;
    instance_data.id = tmpl.id;
    instance_data.obstructable = tmpl.obstructable;
    instance_data.position = position;

    return instance_data;
}

