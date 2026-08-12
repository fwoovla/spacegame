#include "systemobject.hpp"
#include "../../game.h"

SystemLocation::SystemLocation(SystemLocationData *_data) {
    location_data = _data;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = location_data->position;
    info_area.radius = location_data->system_radius;
    info_area.priority = PRIORITY_LOCATION;

    info_area.body_payload = location_data->body_uid;
    info_area.location_payload = location_data->uid;

    CreateLabel(info_label, location_data->position, 40, WHITE, location_data->name.c_str());

}


void SystemLocation::Update() {

}

void SystemLocation::Draw() {
    DrawCircleV(location_data->position, location_data->system_radius, BROWN);

    for(int r = 0; r < 360; r +=10) {

        Vector2 end_pos;
        end_pos.x = location_data->position.x + cosf(r * DEG2RAD) * location_data->system_radius;
        end_pos.y = location_data->position.y + sinf(r * DEG2RAD) * location_data->system_radius;


        Color color = RED;

        if(info_area.mouse_hovering) {
            color =GREEN;
        }


        DrawLineV(location_data->position,  end_pos, color);

    }


}

void SystemLocation::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        Vector2 screen = GetWorldToScreen2D(location_data->position, g_camera);
        info_label.position = screen;
        Color color = DARKGRAY;
        if(info_area.selected) {
            color = GRAY;
        }
        DrawLabelCenteredWithBG(info_label, g_font, color);
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
