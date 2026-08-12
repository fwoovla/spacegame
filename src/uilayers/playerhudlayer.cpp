/*     #include "uilayers.hpp"
    #include "../game.h"
    #include <string>
    
    
    


PlayerHUDLayer::PlayerHUDLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    Vector2 top_left = {20.0f, 20.0f};


    ship_pos = {20.0f, 20.0f};
    target_pos = {g_viewport.resolution.x - 200, 20.0f};


    Vector2 spacing = {0, 40};
    int size = 30;

    CreateLabel(ship_target_label, ship_pos, size, RAYWHITE, "target");
    CreateLabel(speed_label, Vector2Add(ship_target_label.position, spacing), 40, RAYWHITE, "speed");
    CreateLabel(heading_label, Vector2Add(speed_label.position, spacing), size, RAYWHITE, "heading");
    CreateLabel(throttle_label, Vector2Add(heading_label.position, spacing), size, RAYWHITE, "throttle");
    CreateLabel(autopilot_state_label, Vector2Add(throttle_label.position, spacing), size, RAYWHITE, "autopilot state");
    CreateLabel(altitude_label, Vector2Add(autopilot_state_label.position, spacing), size, RAYWHITE, "altitude");



    CreateLabel(target_label, target_pos, size, RAYWHITE, "target");
    CreateLabel(distance_label, Vector2Add(target_label.position, spacing), size, RAYWHITE, "distance");
    CreateLabel(eta_label, Vector2Add(distance_label.position, spacing), size, RAYWHITE, "eta");

}


PlayerHUDLayer::~PlayerHUDLayer() {


}


void PlayerHUDLayer::Draw() {

    DrawLabel(ship_target_label, g_font);
    DrawLabel(speed_label, g_font);
    DrawLabel(heading_label, g_font);
    DrawLabel(throttle_label, g_font);
    DrawLabel(distance_label, g_font);
    DrawLabel(autopilot_state_label, g_font);
    DrawLabel(altitude_label, g_font);
    
    DrawLabel(target_label, g_font);
    DrawLabel(distance_label, g_font);
    //DrawLabel(eta_label, g_font);
    //DrawLineV(entity);

    if(g_game_data.show_debug) {
        Vector2 p1 = GetWorldToScreen2D(g_current_player->entity_data->position, g_camera);

        Vector2 p2;
        if(site != nullptr) {
            p2 = GetWorldToScreen2D(site->position, g_camera);
        }

        if(location != nullptr) {
            p2 = GetWorldToScreen2D(location->position, g_camera);
        }

        if(body != nullptr) {
            p2 = GetWorldToScreen2D(body->position, g_camera);
        }

        DrawLineV(p1, p2, GREEN);

        //printf("p1 %0.5f    p2 %0.5f\n", p1, p2);
    }

}



void PlayerHUDLayer::Update() {
    if(entity != nullptr and selection_manager != nullptr) {
        speed_label.text = "speed: " + std::to_string(entity->ship->ship_data.movement.velocity.x);
        heading_label.text = "heading: " + std::to_string(entity->ship->ship_data.movement.rotation);
        throttle_label.text = "throttle: " + std::to_string(entity->ship->ship_data.movement.throttle);
        ship_target_label.text = "hud target: " + entity->entity_data->name;
        distance_label.text = "distance: " + std::to_string(entity->ship->ship_data.movement.max_speed);
        autopilot_state_label.text = "autopilot state: " + std::to_string(entity->ship->ship_data.movement.drag);
        

        if(site != nullptr) {
            target_label.text = site->name;
            std::string dist = TextFormat("%0.4f", Vector2Distance(entity->entity_data->position, site->position));
            distance_label.text = dist;
        }

        if(location != nullptr) {
            target_label.text = location->name;
            std::string dist = TextFormat("%0.4f", Vector2Distance(entity->entity_data->position, location->position));
            distance_label.text = dist;
        }

        if(body != nullptr) {
            target_label.text = body->name;
            std::string dist = TextFormat("%0.4f", Vector2Distance(entity->entity_data->position, body->position));
            distance_label.text = dist;
        }





    } else {
        speed_label.text = "speed: N/A";
        heading_label.text = "heading: N/A";
        throttle_label.text = "throttle: N/A";
        ship_target_label.text = "target: N/A";
        distance_label.text = "distance: N/A";
        eta_label.text = "eta: N/A";
        autopilot_state_label.text = "autopilot state: N/A";
        altitude_label.text = "altitude: N/A";
        target_label.text = "selected target name: N/A";
    }
}


void PlayerHUDLayer::SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm) {
    entity = _entity;
    selection_manager = sm;
    selection_manager->selected.Connect([this]() { OnBodySelected();});
    selection_manager->deselected.Connect([this]() { OnBodyDeSelected();});
    system = sys;

}

void PlayerHUDLayer::ClearTarget() {
    entity = nullptr;
    selection_manager = nullptr;
    system = nullptr;
    body = nullptr;
    location = nullptr;
    site = nullptr;
}

void PlayerHUDLayer::OnBodySelected() {

    if(selection_manager->selection) {
        MouseTriggerArea *selected_area = selection_manager->selection;
        
        if(selected_area->landing_site_payload != -1) {
            site = &system->map_data.sites[selected_area->landing_site_payload];
        }
        else if(selected_area->location_payload != -1) {
            location = &system->map_data.locations[selected_area->location_payload];
        }
        else if(selected_area->body_payload != -1) {
            body = &system->map_data.bodies[selected_area->body_payload];
        }
    }

}


void PlayerHUDLayer::OnBodyDeSelected() {

    body = nullptr;
    location = nullptr;
    site = nullptr;

} */