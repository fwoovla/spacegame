#include "flightcontrol.hpp"
#include "../../game.h"

FlightControl::FlightControl() {
    components.clear();

//=====================================================

    Rectangle navigation_min_bounds = {30, g_viewport.resolution.y - 110, 200, 100 };

    Rectangle navigation_max_bounds = {30, (g_viewport.resolution.y/2) - 200, 600, 400 };

    std::unique_ptr<Navigation> nav = std::make_unique<Navigation>( navigation_min_bounds, navigation_max_bounds);
    navigation = nav.get();
    components.push_back(std::move(nav));
    //nav->top_panel.button_payload = &nav->top_panel_payload;
    
//=====================================================
    
    Rectangle target_screen_min_bounds = {(g_viewport.resolution.x/2) - 60, g_viewport.resolution.y - 110, 200, 100 };
    Rectangle target_screen_max_bounds = { (g_viewport.resolution.x/2) - 200, (g_viewport.resolution.y/2) - 200, 400, 400 };

    std::unique_ptr<TargetScreen> ts = std::make_unique<TargetScreen>( target_screen_min_bounds, target_screen_max_bounds);
    target_screen = ts.get();
    components.push_back(std::move(ts));
//=====================================================

    Rectangle ship_info_min_bounds = { g_viewport.resolution.x- 230, g_viewport.resolution.y - 110, 200, 100};
    Rectangle ship_info_max_bounds = { g_viewport.resolution.x - 650, (g_viewport.resolution.y/2) - 200, 600, 400 };

    std::unique_ptr<ShipInfo> info = std::make_unique<ShipInfo>( ship_info_min_bounds, ship_info_max_bounds);
    ship_info = info.get();
    components.push_back(std::move(info));
//=====================================================


    autopiolot_indicator = {
        .x = 250,
        .y = g_viewport.resolution.y - 100,
        .width = 20,
        .height = 20
    };
    CreateLabel(autopilot_label, {autopiolot_indicator.x + 25, autopiolot_indicator.y}, 30, WHITE, "autopilot (R)");


    flight_assist_indicator = {
        .x = 250,
        .y = g_viewport.resolution.y - 75,
        .width = 20,
        .height = 20
    };
    CreateLabel(flight_assist_label, {flight_assist_indicator.x + 25, flight_assist_indicator.y}, 30, WHITE, "flight assist (E)");


    CreateLabel(throttle_label, {800, g_viewport.resolution.y - 100}, 30, RAYWHITE, "throttle");
    CreateLabel(speed_label, Vector2Add(throttle_label.position, {0, 30}), 30, RAYWHITE, "speed");

    flight_mode_indicator = {
        .x = 250,
        .y = g_viewport.resolution.y - 50,
        .width = 20,
        .height = 20
    };
    CreateLabel(flight_mode_label, {flight_mode_indicator.x + 25, flight_mode_indicator.y}, 30, WHITE, "flight mode (X/C)");


}


FlightControl::~FlightControl() {

}


void FlightControl::Update() {

    if(entity == nullptr) {
        return;
    }

    shared_nav_data.this_position = entity->entity_data->position;
    shared_target_data.this_position = entity->entity_data->position;

    if(g_input.keys_pressed[0] == KEY_ONE) {
        if(focused_component) {
            focused_component->state = COMPONENT_STATE::MINIMIZED;
            g_input.in_use = false;
        }
        FlightComponent *next = components[0].get();
        if(focused_component != next) {   
            focused_component = components[0].get();
            focused_component->state = COMPONENT_STATE::FOCUSED;
            g_input.in_use = true;
        }
        else {
            focused_component = nullptr;
            g_input.in_use = false;
        }
    }
    if(g_input.keys_pressed[0] == KEY_TWO) {
        if(focused_component) {
            focused_component->state = COMPONENT_STATE::MINIMIZED;
            g_input.in_use = false;
        }
        FlightComponent *next = components[1].get();
        if(focused_component != next) {   
            focused_component = components[1].get();
            focused_component->state = COMPONENT_STATE::FOCUSED;
            g_input.in_use = true;
        }
        else {
            focused_component = nullptr;
            g_input.in_use = false;

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
        SetAutopilotTarget();
    }


    if(g_input.keys_pressed[0] == KEY_V) {
        shared_nav_data = shared_target_data;
        SetAutopilotTarget();
    }

    if(g_input.keys_pressed[0] == KEY_E) {
        entity->ship->ToggleFlightAssist();
    }

    if(g_input.keys_pressed[0] == KEY_X) {
        entity->ship->SetFlightMode(SYSTEM_FLIGHT_MODE);
    }
    if(g_input.keys_pressed[0] == KEY_C) {
        entity->ship->SetFlightMode(LOCAL_FLIGHT_MODE);
    }

    if(g_input.keys_pressed[0] == KEY_J) {
        system->system_travel_requested.EmitSignal();
    }


    std::string  throttle = TextFormat("%0.2f", entity->ship->current_mode->throttle );
    throttle_label.text = throttle + " %";

    std::string speed = TextFormat("%0.4f km/s", entity->ship->current_mode->speed);
    

    speed_label.text = speed;

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
    DrawRectangleLines(autopiolot_indicator.x, autopiolot_indicator.y, autopiolot_indicator.width, autopiolot_indicator.height, WHITE);
    DrawLabel(autopilot_label, g_font);

    Color fa_color = RED;
    if(entity->ship->flight_assist_on) {
        fa_color = GREEN;
    }
    DrawRectangleRec(flight_assist_indicator, fa_color);
    DrawRectangleLines(flight_assist_indicator.x, flight_assist_indicator.y, flight_assist_indicator.width, flight_assist_indicator.height, WHITE);
    DrawLabel(flight_assist_label, g_font);

    DrawLabel(throttle_label, g_font);
    DrawLabel(speed_label, g_font);
    
    Color fm_color = BLUE;
    if(entity->ship_data.flight_mode == SYSTEM_FLIGHT_MODE) {
        fm_color = ORANGE;
    }
    
    DrawRectangleRec(flight_mode_indicator, fm_color);
    DrawRectangleLines(flight_mode_indicator.x, flight_mode_indicator.y, flight_mode_indicator.width, flight_mode_indicator.height, WHITE);
    DrawLabel(flight_mode_label, g_font);

//================================
    if(entity->ship->autopilot_on) {
        Vector2 position;
        float radius;

        if(entity->ship->autopilot.target_data.set) {
            position = entity->ship->autopilot.target_data.position;
            radius = entity->ship->autopilot.target_data.proximity_radius;
            position = GetWorldToScreen2D(position, g_camera);

            radius = radius * g_camera.zoom;
            DrawCircleLinesV(position, radius, WHITE);
            //printf("%0.4f %0.4f %0.4f\n", position.x, position.y, radius);
        }
    }
}


void FlightControl::SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm, std::unordered_map<int, SystemMapData> *_universe_map) {

    shared_nav_data = {};
    shared_target_data = {};

    entity = _entity; //only need entity for potition.  maybe just get ship and figure out a better way?
    selection_manager = sm;
    selection_manager->selected.Connect([this]() { OnTargetSelected();});
    selection_manager->deselected.Connect([this]() { OnTargetDeSelected();});
    system = sys;
    universe_map = _universe_map;

    navigation->CreateSystemList(system);
    navigation->universe_panel.CreateUniverseList(universe_map, system->system_data.uid);

    navigation->SetSharedData(&shared_nav_data, system->system_data.uid);


    navigation->deselect_nav_target.Connect([this]() { OnNavTargetDeSelected();});
    navigation->set_nav_target.Connect([this]() { OnNavTargetSelected();});
    target_screen->target_data = &shared_target_data;

    entity->ship->autopilot.enter_local_space.Connect( [this]() { OnEnterTargetSpace();} );
    entity->ship->autopilot.landing_at_target.Connect( [this]() { OnLandingAtTarget();} );
    entity->ship->autopilot.initiate_autopilot.Connect( [this]() { OnAutopilotInitiated();} );

}



void FlightControl::ClearTarget() {
    entity = nullptr;
    selection_manager = nullptr;
    system = nullptr;

/*     shared_nav_data.site = nullptr;
    shared_nav_data.location = nullptr;
    shared_nav_data.body = nullptr; */

}

void FlightControl::OnTargetSelected() {

    if(selection_manager->selection) {
         MouseTriggerArea *selected_area = selection_manager->selection;
        
        if(selected_area->landing_site_payload != -1) {

            shared_target_data.site = &system->map_data.sites[selected_area->landing_site_payload];
            shared_target_data.location = &system->map_data.locations[selected_area->location_payload];
            shared_target_data.body = &system->map_data.bodies[selected_area->body_payload];
            shared_target_data.set = true;
        }
        else if(selected_area->location_payload != -1) {
            shared_target_data.location = &system->map_data.locations[selected_area->location_payload];
            shared_target_data.body = &system->map_data.bodies[selected_area->body_payload];
            shared_target_data.set = true;
        }
        else if(selected_area->body_payload != -1) {
            shared_target_data.body = &system->map_data.bodies[selected_area->body_payload];
            shared_target_data.set = true;
        }
    }
}



void FlightControl::OnTargetDeSelected() {

    shared_target_data.body = nullptr;
    shared_target_data.location = nullptr;
    shared_target_data.site = nullptr;
    shared_target_data.set = false;

}


void FlightControl::OnNavTargetSelected() {

    SetAutopilotTarget();
    //printf("nav target selected\n");

}


void FlightControl::OnNavTargetDeSelected() {

    MouseTriggerArea *area = nullptr;

    if(shared_nav_data.site != nullptr) {
        area = &shared_nav_data.site->site_instance->info_area;        
    }
    else if(shared_nav_data.location != nullptr) {
        area = &shared_nav_data.location->location_instance->info_area;
    }
    else if(shared_nav_data.body != nullptr) {            
        area = &shared_nav_data.body->body_instance->info_area;
    }

    if(area) {
        area->selected = false;
    }

    shared_nav_data.body = nullptr;
    shared_nav_data.location = nullptr;
    shared_nav_data.site = nullptr;
    shared_nav_data.set = false;

    //printf("area deselected\n");

}


void FlightControl::OnEnterTargetSpace() {
    if(shared_nav_data.body) {system->SetCameraState(CAMERA_BODY);}
    else if(shared_nav_data.location) {system->SetCameraState(CAMERA_LOCATION);}
    else if(shared_nav_data.site) {system->SetCameraState(CAMERA_LOCATION);}
    printf("fc: arived\n");
}

void FlightControl::OnLandingAtTarget() {
    //if()
    system->SetCameraState(CAMERA_SITE);
    printf("fc: landed\n");
    g_game_data.transition.body_id = entity->ship->autopilot.target_data.body_uid;
    g_game_data.transition.location_id = entity->ship->autopilot.target_data.location_uid;
    g_game_data.transition.site_id = entity->ship->autopilot.target_data.site_uid;

    OnNavTargetDeSelected();
    system->landing_requested.EmitSignal();
}

void FlightControl::OnAutopilotInitiated() {
    printf("fc: autopilot engaged\n");

}

void FlightControl::SetAutopilotTarget() {

    AutopilotTarget new_target;
    if(shared_nav_data.site != nullptr) {
        new_target.set = true;
        new_target.position = shared_nav_data.site->position;
        new_target.auto_land = true;
        new_target.proximity_radius = shared_nav_data.site->detect_radius;
        new_target.site_uid = shared_nav_data.site->uid;
        new_target.location_uid = shared_nav_data.site->location_uid;
        new_target.body_uid = shared_nav_data.site->body_uid;
    }
    else if(shared_nav_data.location != nullptr) {
        new_target.set = true;
        new_target.position = shared_nav_data.location->position;
        new_target.auto_land = false;
        new_target.proximity_radius = shared_nav_data.location->detect_radius;
        new_target.location_uid = shared_nav_data.location->uid;
        new_target.body_uid = shared_nav_data.location->body_uid;
    }
    else if(shared_nav_data.body != nullptr) {            
        new_target.set = true;
        new_target.position = shared_nav_data.body->position;
        new_target.auto_land = false;
        new_target.proximity_radius = shared_nav_data.body->detect_radius;
        new_target.body_uid = shared_nav_data.body->uid;
    }
    if(Vector2Distance(entity->entity_data->position, new_target.position) > 1000.0f and new_target.set) {
        entity->ship->SetFlightMode(SYSTEM_FLIGHT_MODE);
    }
    else if(new_target.set) {
        entity->ship->SetFlightMode(LOCAL_FLIGHT_MODE);
    }

    entity->ship->ToggleAutoPilot(new_target);
    
    if(entity->ship->autopilot_on) {
        OnAutopilotInitiated();
    }
        
}