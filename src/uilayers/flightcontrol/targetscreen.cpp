#include "flightcontrol.hpp"


TargetScreen::TargetScreen(Rectangle min, Rectangle max) {
    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "target");

    CreateLabel(target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");

    CreateLabel(distance_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 60}, 30, RAYWHITE, "distance");
}

TargetScreen::~TargetScreen() {

}

void TargetScreen::Update() {
    if(target_data == nullptr) {
        return;
    }

    
    if(target_data->site) {
        target_label.text = target_data->site->name;
        float distance_f = Vector2Distance(target_data->this_position, target_data->site->position);
        std::string  distance = TextFormat("%0.2f", distance_f);
        distance_label.text = distance + " km2";
    }
    else if(target_data->location) {
         target_label.text = target_data->location->name;
         target_label.text = target_data->location->name;
        float distance_f = Vector2Distance(target_data->this_position, target_data->location->position);
        std::string  distance = TextFormat("%0.2f", distance_f);
        distance_label.text = distance + " km2";
    }
    else if(target_data->body) {
         target_label.text = target_data->body->name;
         target_label.text = target_data->body->name;
        float distance_f = Vector2Distance(target_data->this_position, target_data->body->position);
        std::string  distance = TextFormat("%0.2f", distance_f);
        distance_label.text = distance + " km2";
    }
    else {
        target_label.text = "N/A";
        distance_label.text = "N/A";
    }
}

void TargetScreen::Draw() {
    state = MINIMIZED;

    Rectangle bounds = min_bounds;
    Color color = RED;
    if(state == FOCUSED) {
        color = GREEN;
        bounds = max_bounds;
    }
    DrawRectangleRounded(bounds, 0.2f, 2, TRANSDARKERGRAY);
    DrawRectangleRoundedLines(bounds, 0.2f, 2, color);

    top_label.position = {(bounds.width/2) + bounds.x, bounds.y + 20};
    DrawLabelCentered(top_label, g_font);

    target_label.position = {(bounds.width/2) + bounds.x, bounds.y + 40};
    DrawLabelCentered(target_label, g_font);

    target_label.position = {(bounds.width/2) + bounds.x, bounds.y + 60};
    DrawLabelCentered(distance_label, g_font);

}


