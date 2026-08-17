#include "location.hpp"
#include "../game.h"



void Location::GenerateLocation(SystemLocationData &map_data) {


    location_data.uid = map_data.uid;

    location_data.radius = map_data.location_radius;
    location_data.seed = GetRandomValue(100, 100000);


}


void Location::Update() {

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }

    std::erase_if(vec, [](const std::unique_ptr<CreatureEntity> &entity){return entity->should_delete;});

}


void Location::Draw() {

    DrawCircleV({0,0}, location_data.radius, DARKGRAY);

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Draw();
    }

}

void Location::DrawWorld() {

    DrawCircleV({0,0}, location_data.radius, DARKGRAY);

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

