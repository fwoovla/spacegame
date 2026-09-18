#include "characterui.hpp"
#include "../../game.h"





CharacterUI::CharacterUI() {
    components.clear();

//=====================================================
    Rectangle navigation_min_bounds = {30, g_viewport.resolution.y - 110, 200, 100 };
    Rectangle navigation_max_bounds = {30, (g_viewport.resolution.y/2) - 200, 600, 400 };

    std::unique_ptr<CharacterNavigation> nav = std::make_unique<CharacterNavigation>( navigation_min_bounds, navigation_max_bounds);
    navigation = nav.get();
    components.push_back(std::move(nav));
    
//=====================================================
    Rectangle c_info_min_bounds = { g_viewport.resolution.x- 230, g_viewport.resolution.y - 110, 200, 100};
    Rectangle c_info_max_bounds = { g_viewport.resolution.x - 650, (g_viewport.resolution.y/2) - 200, 600, 400 };

    std::unique_ptr<CharacterInfo> info = std::make_unique<CharacterInfo>( c_info_min_bounds, c_info_max_bounds);
    character_info = info.get();
    components.push_back(std::move(info));
//=====================================================



}

void CharacterUI::Update() {
    if(entity == nullptr) {
        return;
    }


    if(g_input.keys_pressed[0] == KEY_ONE) {
        if(focused_component) {
            focused_component->SetState(COMPONENT_STATE::MINIMIZED);
            g_input.in_use = false;
        }
        CharacterUIComponent *next = components[0].get();
        if(focused_component != next) {   
            focused_component = components[0].get();
            focused_component->SetState(COMPONENT_STATE::FOCUSED);
            g_input.in_use = true;
        }
        else {
            focused_component = nullptr;
            g_input.in_use = false;
        }
    }
    if(g_input.keys_pressed[0] == KEY_TWO) {
        if(focused_component) {
            focused_component->SetState(COMPONENT_STATE::MINIMIZED);
            g_input.in_use = false;
        }
        CharacterUIComponent *next = components[1].get();
        if(focused_component != next) {   
            focused_component = components[1].get();
            focused_component->SetState(COMPONENT_STATE::FOCUSED);
            g_input.in_use = true;
        }
        else {
            focused_component = nullptr;
            g_input.in_use = false;

        }
    }


    for(auto &component : components) {
        component->Update();
    }

}

void CharacterUI::Draw() {
    Rectangle bg_rect = {
        .x = 0,
        .y = g_viewport.resolution.y - 120,
        .width = g_viewport.resolution.x,
        .height = g_viewport.resolution.y
    };

    DrawRectangleRec(bg_rect, DARKERGRAY);

    for(auto &component :components) {
        component->Draw();
    }
}


void CharacterUI::SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm, UniverseData *_universe) {

    entity = _entity; 
    character_info->character = entity->character.get();
    selection_manager = sm;
    selection_manager->selected.Connect([this]() { OnTargetSelected();});
    selection_manager->deselected.Connect([this]() { OnTargetDeSelected();});
    system = sys;
    universe = _universe;
    universe_map = &_universe->map_data;

}



void CharacterUI::ClearTarget() {
    entity = nullptr;
    selection_manager = nullptr;
    system = nullptr;


}



void CharacterUI::OnTargetSelected() {

    if(!selection_manager) {
        return;
    } 

}



void CharacterUI::OnTargetDeSelected() {



}

