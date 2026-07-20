#include "universe.hpp"
#include "../game.h"


LocationManager::LocationManager() {

    location_data.launch_site = std::make_unique<TransitionSite>();
    location_data.launch_site->position = {0,0};
    location_data.launch_site->transition_area = std::make_unique<TransitionArea>();
    location_data.launch_site->transition_area->size = {100, 100};
    location_data.launch_site->transition_area->position = {-50 ,-50};
    location_data.launch_site->transition_area->type = LAUNCHING;
    location_data.launch_site->transition_area->area_activated.Connect([&]() { OnLaunchRequested();});

}


LocationManager::~LocationManager() {

}

void LocationManager::CreateLocation() {

}
//void UpdateBodies(Vector2 target_pos);
void LocationManager::Update(Vector2 player_position) {
    //printf("location manager update\n");

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }
    std::erase_if(vec, [](const std::unique_ptr<BaseEntity> &entity){return entity->should_delete;});

    location_data.launch_site->transition_area->Update();

}

void LocationManager::Draw() {
    BeginMode2D(g_camera);

    DrawCircleV({0,0}, location_data.radius, DARKGRAY);
    DrawCircle( location_data.launch_site->position.x, location_data.launch_site->position.y, location_data.launch_site->transition_area->size.x, RED);

    auto &vec = location_data.entity_list;

    for (auto &entity : vec) {
        entity->Draw();
    }
    EndMode2D();
}


void LocationManager::DrawUI() {

    for(auto &entity : location_data.entity_list) {
        entity->DrawUI();
    }
    
    location_data.launch_site->transition_area->Draw();
}

void LocationManager::OnLaunchRequested() {

    launch_requested.EmitSignal();
}