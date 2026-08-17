#include "systemobject.hpp"
#include "../../game.h"


#define DETECT_RADIUS_FACTOR 100.5f

SystemSite::SystemSite(SystemSiteData *_data) {
    site_data = _data;
    site_data->site_instance = this;

    site_data->detect_radius = site_data->system_radius * DETECT_RADIUS_FACTOR;

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
    DrawCircleV(site_data->position, site_data->system_radius, DARKGRAY);

    
}

void SystemSite::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        
        Vector2 top = site_data->position;
        top.y -= site_data->system_radius - 20;
        
        top = GetWorldToScreen2D(top, g_camera);
        if(info_area.mouse_hovering) {
            info_label.position = top;
            DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
        }
        if(info_area.selected) {
            Vector2 center = GetWorldToScreen2D(site_data->position, g_camera);
            DrawCircleLinesV(center, site_data->system_radius * g_camera.zoom, GREEN);
        }
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