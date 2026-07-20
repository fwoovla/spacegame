#include "scenes.hpp"
#include "../game.h"


LocationScene::LocationScene() {
    location_manager.location_data.radius = 400;

}

 SCENE_ID LocationScene::Update() {
    
    HandleCamera();
    location_manager.Update(g_current_player->entity_data->position);

    
    return SCENE_NONE;
 
 }

 void LocationScene::DrawScene() {
    location_manager.Draw();
    
 }

 void LocationScene::DrawUI() {
    location_manager.DrawUI();
    
 }

void LocationScene::OnWorldTick() {

}