#include "entity.hpp"
#include "../game.h"

SystemBodyEntity::SystemBodyEntity(SystemBodyData *_data) {
    body_data = _data;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = body_data->position;
    info_area.radius = body_data->radius;
    info_area.priority = PRIORITY_BODY;

    info_area.body_payload = body_data->uid;

    //info_area.mouse_entered.Connect( [this]() { OnShowInfo();});
    //info_area.mouse_exited.Connect( [this]() { OnHideInfo();});
    //info_area.mouse_triggered.Connect();

    CreateLabel(info_label, body_data->position, 40, WHITE, body_data->name.c_str());

}

/* 
void SystemBodyEntity::GenerateLocation(LocationMapData &location) {

    BodyLocation new_location;
    new_location.name = location.name;
    new_location.uid = location.uid;
    new_location.radius = location.system_radius;
    new_location.position = location.position;
    CreateLabel(new_location.info_label, {0,0}, 50, GRAY, new_location.name.c_str());

    

    new_location.info_area.shape = MouseTriggerArea::CIRCLE;
    new_location.info_area.position = new_location.position;
    new_location.info_area.radius = new_location.radius;
    new_location.info_area.priority = PRIORITY_LOCATION;

    new_location.info_area.location_payload = new_location.uid;
    new_location.info_area.body_payload = body_data->uid;

    CreateLabel(new_location.info_label, new_location.position, 40, WHITE, new_location.name.c_str());

    for(auto & site : location.landing_sites) {
        LandingSite new_site;
        new_site.uid = GetUID();
        new_site.name = site.name;

        new_site.position = site.position;

        new_site.info_area.shape = MouseTriggerArea::CIRCLE;
        new_site.info_area.position = new_site.position;
        new_site.info_area.radius = 10;
        new_site.info_area.priority = PRIORITY_LANDING_SITE;

        new_site.info_area.body_payload = body_data->uid;
        new_site.info_area.location_payload = location.uid;
        new_site.info_area.landing_site_payload = new_site.uid;

        CreateLabel(new_site.info_label, {0,0}, 50, RAYWHITE, new_site.name.c_str());

        printf("new site instance    name: %s    uid:  %i\n", new_site.info_label.text.c_str(), new_site.uid);
        new_location.landing_sites.push_back(std::move(new_site));

        //new_site.info_area.mouse_triggered.Connect(selection_callback);
    }
    
    locations.push_back(new_location);


}

    
 */


void SystemBodyEntity::Update() {

}

void SystemBodyEntity::Draw() {
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
        DrawLineV(body_data->position,  parent->body_data->position, RED);
    }

    /* for(auto &location : locations) {

        Color l_color = DARKBROWN;
        if(location.info_area.mouse_hovering) {
            l_color = BROWN;
        }
        DrawCircleV(location.position, location.radius, l_color);

        for(auto &site : location.landing_sites) {
            Color color = RED;
            if(site.info_area.mouse_hovering) {
                color = GREEN;
            }
            DrawCircleV(site.position, site.info_area.radius, color);
        }
    } */
}

void SystemBodyEntity::DrawOverlay() {
    

/*     for(auto &location : locations) {
        if(location.info_area.mouse_hovering or location.info_area.selected) {
            Vector2 screen = GetWorldToScreen2D(location.position, g_camera);
            location.info_label.position = screen;
            location.info_label.position.y -= 50;
            Color color = DARKGRAY;
            if(location.info_area.selected) {
                color = GRAY;
            }
            DrawLabelCenteredWithBG(location.info_label, g_font, color);
        }
    }


    for(auto &location : locations) {
        for(auto &site : location.landing_sites) {
            if(site.info_area.mouse_hovering or site.info_area.selected) {
                Vector2 screen = GetWorldToScreen2D(site.position, g_camera);
                site.info_label.position = screen;
                site.info_label.position.y -= 50;
                Color color = DARKGRAY;
                if(site.info_area.selected) {
                    color = GRAY;
                }
                DrawLabelCenteredWithBG(site.info_label, g_font, color);
            }
        }
    } */

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

void SystemBodyEntity::DrawUI() {
/*     for(auto &site : landing_sites) {
        site->transition_area->Draw();
    } */
}

float SystemBodyEntity::GetRenderScale() {

    return 1.0f;
}

void SystemBodyEntity::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);
/*     for(auto &location : locations) {
        selection_manager->Register(&location.info_area);
        for(auto &site : location.landing_sites) {
            selection_manager->Register(&site.info_area);
            
        }
    } */
}


