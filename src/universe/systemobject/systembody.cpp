#include "systemobject.hpp"
#include "../../game.h"

#define DETECT_RADIUS_FACTOR 2.5f

SystemBody::SystemBody(SystemBodyData *_data) {
    body_data = _data;
    body_data->body_instance = this;

    body_data->detect_radius = body_data->radius * DETECT_RADIUS_FACTOR;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = body_data->position;
    info_area.radius = body_data->radius;
    info_area.priority = PRIORITY_BODY;

    info_area.body_payload = body_data->uid;

    CreateLabel(info_label, body_data->position, 40, WHITE, body_data->name.c_str());

}


void SystemBody::Update() {

}

void SystemBody::Draw() {
    DrawCircleV(body_data->position, body_data->radius, body_data->modulate);

    if(body_data->parent_uid != body_data->uid and g_game_data.show_debug) {
        DrawLineV(body_data->position,  parent_data->position, RED);
    }

}

void SystemBody::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        
        Vector2 top = body_data->position;
        top.y -= body_data->radius - 20;
        
        top = GetWorldToScreen2D(top, g_camera);
        if(info_area.mouse_hovering) {
            info_label.position = top;
            DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
        }
        
        if(info_area.selected) {
            Vector2 center = GetWorldToScreen2D(body_data->position, g_camera);
            DrawCircleLinesV(center, (body_data->radius * 1.25f) * g_camera.zoom, GREEN);
        }
    }
    

}

void SystemBody::DrawUI() {

}

float SystemBody::GetRenderScale() {

    return 1.0f;
}

void SystemBody::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}


