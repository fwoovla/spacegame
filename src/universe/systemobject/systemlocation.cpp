#include "systemobject.hpp"
#include "../../game.h"

#define DETECT_RADIUS_FACTOR 5.5f

SystemLocation::SystemLocation(SystemLocationData *_data) {
    location_data = _data;
    location_data->location_instance = this;

    location_data->detect_radius = location_data->radius * DETECT_RADIUS_FACTOR;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = location_data->position;
    info_area.radius = location_data->radius;
    info_area.priority = PRIORITY_LOCATION;

    info_area.body_payload = location_data->body_uid;
    info_area.location_payload = location_data->uid;

    CreateLabel(info_label, location_data->position, 40, WHITE, location_data->name.c_str());

}


void SystemLocation::Update() {

}

void SystemLocation::Draw() {
    DrawCircleV(location_data->position, location_data->radius, BROWN);

    Color color = RED;

    if(info_area.mouse_hovering) {
        color =GREEN;
    }

    DrawCircleLinesV(location_data->position, location_data->radius + 10, color);




}

void SystemLocation::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        
        Vector2 top = location_data->position;
        top.y -= location_data->radius - 20;
        
        top = GetWorldToScreen2D(top, g_camera);
        if(info_area.mouse_hovering) {
            info_label.position = top;
            DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
        }
        if(info_area.selected) {
            Vector2 center = GetWorldToScreen2D(location_data->position, g_camera);
            DrawCircleLinesV(center, location_data->radius * g_camera.zoom, GREEN);
        }
    }
    
}

void SystemLocation::DrawUI() {

}

float SystemLocation::GetRenderScale() {

    return 1.0f;
}

void SystemLocation::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}
