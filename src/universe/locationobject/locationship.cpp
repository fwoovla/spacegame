#include "locationobject.hpp"
#include "../../game.h"


#define DETECT_RADIUS_FACTOR 1.5f

LocationShip::LocationShip(ShipData *_data, Vector2 position) {
    ship_data = _data;
    this->position = position;
    //site_data->site_instance = this;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = position;
    info_area.radius = ship_data->radius ;
    info_area.priority = PRIORITY_NPC;
    //info_area.selected = true;
    //info_area.mouse_hovering = true;

    CreateLabel(info_label, position, 40, WHITE, ship_data->name.c_str());

}




void LocationShip::Update() {
    float detect_radius = ship_data->radius * DETECT_RADIUS_FACTOR;
    if(
        CheckCollisionPointCircle(g_current_player->entity_data->position, position, detect_radius) and info_area.selected) {
        can_board_ship = true;
    }
    else {
        can_board_ship = false;
    }

}

void LocationShip::Draw() {
    Color color = PURPLE;
    if(can_board_ship) {
        color = PINK;
    }
    DrawCircleV(position, ship_data->radius, color);
    //printf("draw ship radius: %0.5f\n", ship_data->radius);

    
}

void LocationShip::DrawOverlay() {

    //DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
    if(info_area.mouse_hovering or info_area.selected) {
        
        Vector2 top = position;
        top.y -= ship_data->radius - 20;
        
        top = GetWorldToScreen2D(top, g_camera);
        if(info_area.mouse_hovering) {
            info_label.position = top;
            DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
        }
        if(info_area.selected) {
            
            Vector2 center = GetWorldToScreen2D(position, g_camera);
            DrawCircleLinesV(center, (ship_data->radius * 1.3) * g_camera.zoom, GREEN);
        }
        
    }
}

void LocationShip::DrawUI() {

}

float LocationShip::GetRenderScale() {
    return 1.0f;
}

void LocationShip::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}