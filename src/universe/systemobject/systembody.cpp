#include "systemobject.hpp"
#include "../../game.h"

SystemBody::SystemBody(SystemBodyData *_data) {
    body_data = _data;

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

    for(int r = 0; r < 360; r +=10) {

        Vector2 end_pos;
        end_pos.x = body_data->position.x + cosf(r * DEG2RAD) * body_data->radius;
        end_pos.y = body_data->position.y + sinf(r * DEG2RAD) * body_data->radius;


        Color color = RED;

        if(info_area.mouse_hovering) {
            color =GREEN;
        }


        DrawLineV(body_data->position,  end_pos, color);

    }

    if(body_data->parent_uid != body_data->uid and g_game_data.show_debug) {
        DrawLineV(body_data->position,  parent_data->position, RED);
    }

}

void SystemBody::DrawOverlay() {

    if(info_area.mouse_hovering or info_area.selected) {
        Vector2 screen = GetWorldToScreen2D(body_data->position, g_camera);
        info_label.position = screen;
        Color color = DARKGRAY;
        if(info_area.selected) {
            color = GRAY;
        }
        DrawLabelCenteredWithBG(info_label, g_font, color);
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


