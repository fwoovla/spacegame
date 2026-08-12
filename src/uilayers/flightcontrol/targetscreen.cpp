#include "flightcontrol.hpp"


TargetScreen::TargetScreen(Rectangle min, Rectangle max) {
    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "target");

    CreateLabel(target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");
}

TargetScreen::~TargetScreen() {

}

void TargetScreen::Update() {
    if(nav_target_data == nullptr) {
        return;
    }
    if(nav_target_data->site) {
        target_label.text = nav_target_data->site->name;
    }
    else if(nav_target_data->location) {
         target_label.text = nav_target_data->location->name;
    }
    else if(nav_target_data->body) {
         target_label.text = nav_target_data->body->name;
    }
    else {
        target_label.text = "N/A";
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

}


