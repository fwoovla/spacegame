#include "flightcontrol.hpp"


ShipInfo::ShipInfo(Rectangle min, Rectangle max) {
    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "this ship");

}

ShipInfo::~ShipInfo() {

}

void ShipInfo::Update() {

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

}
