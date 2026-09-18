#include "locationobject.hpp"
#include "../../game.h"


#define DETECT_RADIUS_FACTOR 5.5f

LocationSite::LocationSite(LocationSiteData *_data) {
    site_data = _data;
    site_data->site_instance = this;

    site_data->detect_radius = site_data->radius * 1.5;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = site_data->position;
    info_area.radius = site_data->radius;
    info_area.priority = PRIORITY_LANDING_SITE;

    info_area.landing_site_payload = site_data->uid;
    info_area.location_payload = site_data->location_uid;
    info_area.body_payload = site_data->body_uid;

    CreateLabel(info_label, site_data->position, 40, WHITE, site_data->name.c_str());

}


LocationSite::~LocationSite() {
    if (selection_manager){
        selection_manager->Unregister(&info_area);
    }
}



void LocationSite::Update() {
    is_on_screen = IsOnScreen(site_data->position, site_data->radius);
    if(is_on_screen) {
        g_game_data.debug_data.draw_count +=1;
    }

    if(shared_site_data == nullptr) {
        return;
    }

    if(CheckCollisionPointCircle(g_current_player->entity_data->position, site_data->position, site_data->detect_radius) and info_area.selected) {
        can_open_shop = true;
        if(g_input.keys_pressed[0] == KEY_SPACE) {
            g_game_data.shop_type = site_data->local_data->shop_type;
            shared_site_data->type = site_data->local_data->site_type;
            open_shop.EmitSignal();
        }
    }
    else {
        can_open_shop = false;
    }

}

void LocationSite::Draw() {
    if(!is_on_screen) {
        return;
    }

    Color color = RAYWHITE;

    if(site_data->local_data->site_type == SITE_LANDING) {
        color = GREEN;
    }
    else if(site_data->local_data->site_type == SITE_SHOP) {
        color = ORANGE;
    }

    DrawRectangle(site_data->position.x - site_data->radius, site_data->position.y - site_data->radius, site_data->radius*2, site_data->radius*2, color);

    if(info_area.selected) {
        Color c_color = RED;
        if(can_open_shop) {
            c_color = BLUE;
        }
        DrawCircleLinesV(site_data->position, site_data->detect_radius, c_color);
    }
}

void LocationSite::DrawOverlay() {
    if(!is_on_screen) {
        return;
    }

    if(info_area.mouse_hovering or info_area.selected) {
        
        Vector2 top = site_data->position;
        top.y -= site_data->radius - 20;
        
        top = GetWorldToScreen2D(top, g_camera);
        if(info_area.mouse_hovering) {
            info_label.position = top;
            DrawLabelCenteredWithBG(info_label, g_font, TRANSDARKERGRAY);
        }
        if(info_area.selected) {
            Vector2 center = GetWorldToScreen2D(site_data->position, g_camera);
            DrawCircleLinesV(center, (site_data->radius * 1.25f) * g_camera.zoom, PURPLE);
        }
    }
}

void LocationSite::DrawUI() {

}

float LocationSite::GetRenderScale() {
    return 1.0f;
}

void LocationSite::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    printf("LocationSite selection_manager: %p\n",
       (void*)sm);
    selection_manager->Register(&info_area);

}

