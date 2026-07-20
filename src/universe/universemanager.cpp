#include "universe.hpp"
#include "../game.h"


void UniverseManager::CreateUniverse(std::string player_name) {
    printf("creating a  new universe for player:   %s\n", player_name.c_str());

    current_system = std::make_unique<System>();


    current_system->GenerateSystem();

    


/*     EntityData player_data = GenerateEntityInstance(g_entity_template_data[ENTITY_PLAYER], 0, {500, 500});
    universe_data.system_data.entity_data[player_data.uid] = player_data;
    */
   g_current_player = current_system->SpawnPlayer(g_entity_template_data[ENTITY_PLAYER], 0, {current_system->system_data.radius - 500, current_system->system_data.radius}); 
    
    //g_current_player = player;

    //universe_data.system_data.entity_list.push_back(std::move(player));
    //printf("player created with name  %s\n", g_current_player->entity_data->name.c_str());


}


void UniverseManager::UpdateBodies(Vector2 target_pos) {
    for(auto &body : current_system->system_data.body_list) {
        body->Update();
    }
    //printf("updating chuks\n");

}

void UniverseManager::UpdateEntities() {

    auto &vec = current_system->system_data.entity_list;

        for (auto &entity : vec)
        {
            entity->Update();
        }
        std::erase_if(vec, [](const std::unique_ptr<BaseEntity> &entity){return entity->should_delete;});
}

void UniverseManager::Draw() {

    for(auto &body : current_system->system_data.body_list) {
        body->Draw();
    }
    
    for(auto &entity : current_system->system_data.entity_list) {
        entity->Draw();
    }

}

void UniverseManager::DrawUI() {

    for(auto &entity : current_system->system_data.entity_list) {
        entity->DrawUI();
    }
    for(auto &body : current_system->system_data.body_list) {
        body->DrawUI();
    }

}