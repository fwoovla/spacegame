#include "systemobject.hpp"
#include "../../game.h"

SystemSite::SystemSite(SystemSiteData *_data) {
    site_data = _data;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = site_data->position;
    info_area.radius = site_data->system_radius;
    info_area.priority = PRIORITY_LANDING_SITE;

    info_area.landing_site_payload = site_data->uid;
    info_area.location_payload = site_data->location_uid;
    info_area.body_payload = site_data->body_uid;


    CreateLabel(info_label, site_data->position, 40, WHITE, site_data->name.c_str());

}




void SystemSite::Update() {

}

void SystemSite::Draw() {
    DrawCircleV(site_data->position, site_data->system_radius, BROWN);

    for(int r = 0; r < 360; r +=10) {

        Vector2 end_pos;
        end_pos.x = site_data->position.x + cosf(r * DEG2RAD) * site_data->system_radius;
        end_pos.y = site_data->position.y + sinf(r * DEG2RAD) * site_data->system_radius;


        Color color = RED;

        if(info_area.mouse_hovering) {
            color =GREEN;
        }


        DrawLineV(site_data->position,  end_pos, color);

    }
}

void SystemSite::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        Vector2 screen = GetWorldToScreen2D(site_data->position, g_camera);
        info_label.position = screen;
        Color color = DARKGRAY;
        if(info_area.selected) {
            color = GRAY;
        }
        DrawLabelCenteredWithBG(info_label, g_font, color);
    }

}

void SystemSite::DrawUI() {

}

float SystemSite::GetRenderScale() {
    return 1.0f;
}

void SystemSite::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}