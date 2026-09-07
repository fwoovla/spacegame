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



SystemLocation::~SystemLocation() {
    if (selection_manager){
        selection_manager->Unregister(&info_area);
    }
}


void SystemLocation::Update() {

}

void SystemLocation::Draw() {

    int grid_size = location_data->location_plan.grid_size;
    float size_x = location_data->location_plan.size_x;
    float size_y = location_data->location_plan.size_y;
        
    float lpx = location_data->position.x - ((size_x * 0.5f) * grid_size);
    float lpy = location_data->position.y - ((size_y * 0.5f) * grid_size);
    
    Color color = RED;

    if(info_area.mouse_hovering) {
        color =GREEN;
    }

    DrawRectangle(lpx, lpy, size_x * grid_size, size_y * grid_size, DARKBROWN);

    if(g_game_data.show_debug) {
        for(int y = 0; y <= size_y; y++) {
            DrawLine(lpx + 0, (y * grid_size) + lpy, (size_x * grid_size ) + lpx, (y * grid_size) + lpy, WHITE);
            for(int x = 0; x <= size_x; x++) {
                DrawLine( (x * grid_size) + lpx, 0 + lpy, (x * grid_size) + lpx, (size_y * grid_size) + lpy, WHITE);
            }
        }
        
        for(auto &[uid, pos] : location_data->location_plan.site_locations) {
            DrawRectangle(pos.x + lpx, pos.y + lpy, grid_size, grid_size, DARKBLUE);
        }
    }

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
