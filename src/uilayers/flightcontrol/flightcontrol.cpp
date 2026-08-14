#include "flightcontrol.hpp"
#include "../../game.h"

FlightControl::FlightControl() {
    components.clear();

//=====================================================

    Rectangle navigation_min_bounds = {
        20, g_viewport.resolution.y - 110, 100, 100
    };

    Rectangle navigation_max_bounds = {
        (g_viewport.resolution.x/2) - 200, (g_viewport.resolution.y/2) - 200, 400, 400  
    };

    std::unique_ptr<Navigation> nav = std::make_unique<Navigation>( Navigation(navigation_min_bounds, navigation_max_bounds));
    navigation = nav.get();
    components.push_back(std::move(nav));
    
//=====================================================
    
    Rectangle target_screen_min_bounds = {
        (g_viewport.resolution.x/2) - 60, g_viewport.resolution.y - 110, 100, 100
    };

    Rectangle target_screen_max_bounds = {
        (g_viewport.resolution.x/2) - 200, (g_viewport.resolution.y/2) - 200, 400, 400  
    };

    std::unique_ptr<TargetScreen> ts = std::make_unique<TargetScreen>( TargetScreen(target_screen_min_bounds, target_screen_max_bounds));
    target_screen = ts.get();
    components.push_back(std::move(ts));
//=====================================================
    
    Rectangle ship_info_min_bounds = {
        g_viewport.resolution.x- 120, g_viewport.resolution.y - 110, 100, 100  
    };
    Rectangle ship_info_max_bounds = {
        (g_viewport.resolution.x/2) - 200, (g_viewport.resolution.y/2) - 200, 400, 400  
    };

    std::unique_ptr<ShipInfo> info = std::make_unique<ShipInfo>( ShipInfo(ship_info_min_bounds, ship_info_max_bounds));
    ship_info = info.get();
    components.push_back(std::move(info));
//=====================================================


    autopiolot_indicator = {
        .x = 20,
        .y = 20,
        .width = 20,
        .height = 20
    };
    CreateLabel(autopilot_label, {autopiolot_indicator.x, autopiolot_indicator.y + 20}, 30, WHITE, "autopilot (R)");


    flight_assist_indicator = {
        .x = 20,
        .y = 60,
        .width = 20,
        .height = 20
    };
    CreateLabel(flight_assist_label, {flight_assist_indicator.x, flight_assist_indicator.y + 20}, 30, WHITE, "flight assist (E)");


    CreateLabel(throttle_label, Vector2Add(flight_assist_label.position, {0, 60}), 30, RAYWHITE, "throttle");

    flight_mode_indicator = {
        .x = 20,
        .y = 200,
        .width = 20,
        .height = 20
    };
    CreateLabel(flight_mode_label, {flight_mode_indicator.x, flight_mode_indicator.y + 20}, 30, WHITE, "flight mode (X/C)");


}


FlightControl::~FlightControl() {

}


void FlightControl::Update() {

    if(g_input.keys_pressed[0] == KEY_ONE) {
        if(focused_component) {
            focused_component->state = COMPONENT_STATE::MINIMIZED;
        }
        FlightComponent *next = components[0].get();
        if(focused_component != next) {   
            focused_component = components[0].get();
            focused_component->state = COMPONENT_STATE::FOCUSED;
        }
        else {
            focused_component = nullptr;
        }
    }
    if(g_input.keys_pressed[0] == KEY_TWO) {
        if(focused_component) {
            focused_component->state = COMPONENT_STATE::MINIMIZED;
        }
        FlightComponent *next = components[1].get();
        if(focused_component != next) {   
            focused_component = components[1].get();
            focused_component->state = COMPONENT_STATE::FOCUSED;
        }
        else {
            focused_component = nullptr;
        }

    }
    if(g_input.keys_pressed[0] == KEY_THREE) {
        if(focused_component) {
            focused_component->state = COMPONENT_STATE::MINIMIZED;
        }
        FlightComponent *next = components[2].get();
        if(focused_component != next) {   
            focused_component = components[2].get();
            focused_component->state = COMPONENT_STATE::FOCUSED;
        }
        else {
            focused_component = nullptr;
        }

    }

    if(g_input.keys_pressed[0] == KEY_R) {

        AutopilotTarget new_target;

        if(shared_nav_target_data.site != nullptr) {
            new_target.set = true;
            new_target.position = shared_nav_target_data.site->position;
            new_target.auto_land = true;
        }
        else if(shared_nav_target_data.location != nullptr) {
            new_target.set = true;
            new_target.position = shared_nav_target_data.location->position;
            new_target.auto_land = false;
        }
        else if(shared_nav_target_data.body != nullptr) {            
            new_target.set = true;
            new_target.position = shared_nav_target_data.body->position;
            new_target.auto_land = false;
        }
            
            entity->ship->ToggleAutoPilot(new_target);
    }

    if(g_input.keys_pressed[0] == KEY_E) {
        entity->ship->ToggleFlightAssist();
    }

    if(g_input.keys_pressed[0] == KEY_X) {
        entity->ship->SetFlightMode(SYSTEM_FLIGHT_MODE);
    }
    if(g_input.keys_pressed[0] == KEY_C) {
        entity->ship->SetFlightMode(PLANET_FLIGHT_MODE);
    }

    std::string  throttle = TextFormat("%0.4f", entity->ship->current_mode->throttle);
    throttle_label.text = throttle;

    for(auto &component : components) {
        component->Update();
    }
}

void FlightControl::Draw() {

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

    Color ap_color = RED;
    if(entity->ship->autopilot_on) {
        ap_color = GREEN;
    }
    DrawRectangleRec(autopiolot_indicator, ap_color);
    DrawLabel(autopilot_label, g_font);

    Color fa_color = RED;
    if(entity->ship->flight_assist_on) {
        fa_color = GREEN;
    }
    DrawRectangleRec(flight_assist_indicator, fa_color);
    DrawLabel(flight_assist_label, g_font);
    DrawLabel(throttle_label, g_font);

    Color fm_color = BLUE;
    if(entity->ship->ship_data.flight_mode == SYSTEM_FLIGHT_MODE) {
        fm_color = ORANGE;
    }

    DrawRectangleRec(flight_mode_indicator, fm_color);
    DrawLabel(flight_mode_label, g_font);

    Vector2 p1 = GetWorldToScreen2D(g_current_player->entity_data->position, g_camera);
    Vector2 p2;

    bool draw_line = false;

    if(shared_nav_target_data.site != nullptr) {
        p2 = GetWorldToScreen2D(shared_nav_target_data.site->position, g_camera);
        draw_line = true;
    }
    else if(shared_nav_target_data.location != nullptr) {
        p2 = GetWorldToScreen2D(shared_nav_target_data.location->position, g_camera);
        draw_line = true;
    }
    else if(shared_nav_target_data.body != nullptr) {            
        p2 = GetWorldToScreen2D(shared_nav_target_data.body->position, g_camera);        
        draw_line = true;
    }

    if(draw_line) {
        //DrawLineV(p1, p2, GREEN);
    }
}


void FlightControl::SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm) {
    entity = _entity; //only need entity for potition.  maybe just get ship and figure out a better way?
    selection_manager = sm;
    selection_manager->selected.Connect([this]() { OnSystemObjectSelected();});
    selection_manager->deselected.Connect([this]() { OnSystemObjectDeSelected();});
    system = sys;  //get universe to access outside systems

    navigation->CreateSystemList(system);

    navigation->nav_target_data = &shared_nav_target_data;
    target_screen->nav_target_data = &shared_nav_target_data;

}

void FlightControl::ClearTarget() {
    entity = nullptr;
    selection_manager = nullptr;
    system = nullptr;

    shared_nav_target_data.site = nullptr;
    shared_nav_target_data.location = nullptr;
    shared_nav_target_data.body = nullptr;

}

void FlightControl::OnSystemObjectSelected() {

    if(selection_manager->selection) {
         MouseTriggerArea *selected_area = selection_manager->selection;
        
        if(selected_area->landing_site_payload != -1) {
            shared_nav_target_data.site = &system->map_data.sites[selected_area->landing_site_payload];
            shared_nav_target_data.location = &system->map_data.locations[selected_area->location_payload];
            shared_nav_target_data.body = &system->map_data.bodies[selected_area->body_payload];
        }
        else if(selected_area->location_payload != -1) {
            shared_nav_target_data.location = &system->map_data.locations[selected_area->location_payload];
            shared_nav_target_data.body = &system->map_data.bodies[selected_area->body_payload];
        }
        else if(selected_area->body_payload != -1) {
            shared_nav_target_data.body = &system->map_data.bodies[selected_area->body_payload];
        }
    }
}


void FlightControl::OnSystemObjectDeSelected() {

    shared_nav_target_data.body = nullptr;
    shared_nav_target_data.location = nullptr;
    shared_nav_target_data.site = nullptr;

}