#include "entity.hpp"
#include "../game.h"

SystemBodyEntity::SystemBodyEntity(SystemBodyData *_data) {
    body_data = _data;

    info_area.shape = MouseTriggerArea::CIRCLE;
    info_area.position = body_data->position;
    info_area.radius = body_data->radius;
    info_area.priority = PRIORITY_BODY;

    info_area.mouse_entered.Connect( [&]() { OnShowInfo();});
    info_area.mouse_exited.Connect( [&]() { OnHideInfo();});

    CreateLabel(info_label, body_data->position, 40, WHITE, body_data->name.c_str());

}


void SystemBodyEntity::GenerateLocation(LocationMapData &location) {


    BodyLocation new_location;
    new_location.name = location.name;
    new_location.uid = location.uid;
    new_location.radius = location.system_radius;
    new_location.position = location.position;
    CreateLabel(new_location.label, {0,0}, 50, GRAY, new_location.name.c_str());

    for(auto & site : location.landing_sites) {
        LandingSite new_site;
        new_site.uid = GetUID();
        new_site.name = site.name;
        new_site.position = site.position;

        //new_site.transition_area = std::make_unique<TransitionArea>();
        new_site.transition_area.location_uid = location.uid;
        new_site.transition_area.size = {5, 5};
        new_site.transition_area.position.x = new_site.position.x - (new_site.transition_area.size.x * 0.5f);
        new_site.transition_area.position.y = new_site.position.y - (new_site.transition_area.size.y * 0.5f);
        new_site.transition_area.type = LANDING;
        new_site.transition_area.priority = PRIORITY_LANDING_SITE;

        CreateLabel(new_site.label, {0,0}, 50, GRAY, new_site.name.c_str());
        printf("new site instance    name: %s    uid:  %i\n", new_site.label.text.c_str(), new_site.uid);

        new_location.landing_sites.push_back(std::move(new_site));

    }

    new_location.info_area.shape = MouseTriggerArea::CIRCLE;
    new_location.info_area.position = new_location.position;
    new_location.info_area.radius = new_location.radius;
    new_location.info_area.location_id = new_location.uid;

    new_location.info_area.mouse_entered.Connect( [this]() { OnShowLocationInfo();});
    new_location.info_area.mouse_exited.Connect( [this]() { OnHideLocationInfo();});

    CreateLabel(new_location.label, new_location.position, 40, WHITE, new_location.name.c_str());
    
    locations.push_back(new_location);

}

    



void SystemBodyEntity::Update() {
    for(auto &location : locations) {
        location.info_area.Update();
        for(auto &site : location.landing_sites)
        site.transition_area.Update();
        //printf("landing site\n");
    }
    //info_area.Update();

}

void SystemBodyEntity::Draw() {
    DrawCircleV(body_data->position, body_data->radius, body_data->modulate);

    for(int r = 0; r < 360; r +=10) {

        Vector2 end_pos;
        end_pos.x = body_data->position.x + cosf(r * DEG2RAD) * body_data->radius;
        end_pos.y = body_data->position.y + sinf(r * DEG2RAD) * body_data->radius;


        Color color = RED;

        if(show_info) {
            color =GREEN;
        }


        DrawLineV(body_data->position,  end_pos, color);

    }

    if(body_data->parent != nullptr and g_game_data.show_debug) {
        DrawLineV(body_data->position, body_data->parent->position, RED);
    }

    for(auto &location : locations) {

        Color l_color = DARKBROWN;
        if(location.info_area.hovered) {
            l_color = BROWN;
        }
        DrawCircleV(location.position, location.radius, l_color);

        for(auto &site : location.landing_sites) {
            //DrawCircle(site.position.x, site.position.y, site.transition_area.size.x, GRAY);
            Color color = RED;
            if(site.transition_area.hovered) {
                color = GREEN;
            }
            DrawRectangle(site.transition_area.position.x, site.transition_area.position.y, site.transition_area.size.x, site.transition_area.  size.y, color);
            //printf("landing site\n");
        }
    }
}

void SystemBodyEntity::DrawOverlay() {
    
    if(show_location_info) {
        for(auto &location : locations) {
            if(location.uid == g_game_data.transition.location_id) {
                Vector2 screen = GetWorldToScreen2D(location.position, g_camera);
                location.label.position = screen;
                location.label.position.y -= 50;
                DrawLabelCenteredWithBG(location.label, g_font, DARKGRAY);
            }
        }
    }

    for(auto &location : locations) {

        for(auto &site : location.landing_sites) {
            if(site.transition_area.hovered) {
                Vector2 screen = GetWorldToScreen2D(site.position, g_camera);
                site.label.position = screen;
                //site->label.position.y = location->label.position.y + 40;
                DrawLabelCenteredWithBG(site.label, g_font, DARKGRAY);
                //printf("drawing lsite label    name: %s\n", site->label.text.c_str());
            }
        }
    }

    if(show_info) {
        Vector2 screen = GetWorldToScreen2D(body_data->position, g_camera);
        info_label.position = screen;
        DrawLabelCenteredWithBG(info_label, g_font, DARKGRAY);
        //printf("showing info for planet %s       %0.5f %0.5f\n", info_label.text.c_str(), info_label.position.x, info_label.position.y);
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
    for(auto &location : locations) {
        selection_manager->Register(&location.info_area);
        for(auto &site : location.landing_sites) {
            //site.transition_area.area_activated.Connect([&](){OnTransitionClicked();});
        }
    }
}



void SystemBodyEntity::OnShowInfo() {
    show_info = true;
}


void SystemBodyEntity::OnHideInfo() {
    show_info = false;
}


void SystemBodyEntity::OnShowLocationInfo() {
    if(!show_location_info) {
        show_location_info = true;
    }
}


void SystemBodyEntity::OnHideLocationInfo() {
    if(show_location_info) {
        show_location_info = false;
    }
}