#include "flightcontrol.hpp"


ShipInfo::ShipInfo(Rectangle min, Rectangle max) {
    //ship = _ship;

    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {top_label.position.x, top_label.position.y + 30}, 30, RAYWHITE, "this ship");
    CreateLabel(fuel_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "fuel");

}

ShipInfo::~ShipInfo() {

}

void ShipInfo::Update() {
    if(ship == nullptr) {
        return;
    }

    fuel_label.text = std::to_string(int(ship->ship_data->fuel));
}

void ShipInfo::Draw() {
    
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

    fuel_label.position = {top_label.position.x, top_label.position.y + 30};
    DrawLabelCentered(fuel_label, g_font);

}

void ShipInfo::SetState(COMPONENT_STATE new_state) {
    state = new_state;
}