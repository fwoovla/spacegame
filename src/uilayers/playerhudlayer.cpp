    #include "uilayers.hpp"
    #include "../game.h"
    #include <string>
    
    
    


PlayerHUDLayer::PlayerHUDLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    Vector2 top_left = {20.0f, 20.0f};
    Vector2 spacing = {0, 40};

    CreateLabel(speed_label, top_left, 40, RAYWHITE, "speed");
    CreateLabel(heading_label, Vector2Add(speed_label.position, spacing), 40, RAYWHITE, "heading");
    CreateLabel(throttle_label, Vector2Add(heading_label.position, spacing), 40, RAYWHITE, "throttle");
    CreateLabel(ship_target_label, Vector2Add(throttle_label.position, spacing), 40, RAYWHITE, "target");
    CreateLabel(distance_label, Vector2Add(ship_target_label.position, spacing), 40, RAYWHITE, "distance");
    CreateLabel(eta_label, Vector2Add(distance_label.position, spacing), 40, RAYWHITE, "eta");
    CreateLabel(autopilot_state_label, Vector2Add(eta_label.position, spacing), 40, RAYWHITE, "autopilot state");
    CreateLabel(altitude_label, Vector2Add(autopilot_state_label.position, spacing), 40, RAYWHITE, "altitude");

    CreateLabel(target_label, Vector2Add(altitude_label.position, spacing), 50, RAYWHITE, "target");

}


PlayerHUDLayer::~PlayerHUDLayer() {


}


void PlayerHUDLayer::Draw() {

    DrawLabel(speed_label, g_font);
    DrawLabel(heading_label, g_font);
    DrawLabel(throttle_label, g_font);
    DrawLabel(ship_target_label, g_font);
    DrawLabel(distance_label, g_font);
    DrawLabel(eta_label, g_font);
    DrawLabel(autopilot_state_label, g_font);
    DrawLabel(altitude_label, g_font);

    DrawLabel(target_label, g_font);


}



void PlayerHUDLayer::Update() {
    if(ship_target != nullptr) {
        speed_label.text = "speed: " + std::to_string(ship_target->movement.velocity.x);
        heading_label.text = "heading: " + std::to_string(ship_target->movement.rotation);
        throttle_label.text = "throttle: " + std::to_string(ship_target->movement.throttle);
        ship_target_label.text = "target: " + ship_target->name;
        distance_label.text = "distance: " + std::to_string(ship_target->movement.max_speed);
        eta_label.text = "eta: " + std::to_string(ship_target->movement.turn_speed);
        autopilot_state_label.text = "autopilot state: " + std::to_string(ship_target->movement.drag);
        //altitude_label.text = "altitude: " + std::to_string(ship_target->movement.);
        target_label.text = "target name: " + ship_target->target_data.name;
    } else {
        speed_label.text = "speed: N/A";
        heading_label.text = "heading: N/A";
        throttle_label.text = "throttle: N/A";
        ship_target_label.text = "target: N/A";
        distance_label.text = "distance: N/A";
        eta_label.text = "eta: N/A";
        autopilot_state_label.text = "autopilot state: N/A";
        altitude_label.text = "altitude: N/A";
    }
}


void PlayerHUDLayer::SetTarget(ShipData *ship_data) {
    this->ship_target = ship_data;

}

void PlayerHUDLayer::ClearTarget() {
    this->ship_target = nullptr;
}