#include "flightcontrol.hpp"


Navigation::Navigation(Rectangle min, Rectangle max) {

    min_bounds = min;
    max_bounds = max;

    system_list = SystemList({(max_bounds.width/2) + max_bounds.x, max_bounds.y + 150});

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "navigation");
    CreateLabel(nav_target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");
    
    Button all_button;// = CreateButton(position, size, color, text)
    CreateButton(all_button, {(max_bounds.width/2) + max_bounds.x, max_bounds.y + 80}, {40, 30}, ORANGE,"A");
    all_button.text_color_focus = DARKERGRAY;
    all_button.text_color = GRAY;
    all_button.default_color = DARKRED;
    top_panel.AddButton(all_button, KEY_HOME, "ALL");

    Button planet_button;// = CreateButton(position, size, color, text)
    CreateButton(planet_button, {(max_bounds.width/2) + max_bounds.x - 50, max_bounds.y + 115}, {40, 30}, ORANGE,"P");
    planet_button.text_color_focus = DARKERGRAY;
    planet_button.text_color = GRAY;
    planet_button.default_color = DARKRED;
    top_panel.AddButton(planet_button, KEY_DELETE, "BODIES");

    Button location_button;// = CreateButton(position, size, color, text)
    CreateButton(location_button, {(max_bounds.width/2) + max_bounds.x, max_bounds.y + 115}, {40, 30}, ORANGE,"L");
    location_button.text_color_focus = DARKERGRAY;
    location_button.text_color = GRAY;
    location_button.default_color = DARKRED;
    top_panel.AddButton(location_button, KEY_END, "LOCATIONS");

    Button site_button;// = CreateButton(position, size, color, text)
    CreateButton(site_button, {(max_bounds.width/2) + max_bounds.x + 50, max_bounds.y + 115}, {40, 30}, ORANGE,"S");
    site_button.text_color_focus = DARKERGRAY;
    site_button.text_color = GRAY;
    site_button.default_color = DARKRED;
    top_panel.AddButton(site_button, KEY_PAGE_DOWN, "SITES");

    top_panel.button_payload = &top_panel_payload;
    top_panel.button_pressed.Connect([this]() { OnTopPanelButtonPressed();});

}

Navigation::~Navigation() {

}

void Navigation::Update() {
    if(nav_target_data == nullptr) {
        return;
    }

    if(state == FOCUSED) {
        if(nav_target_data->site) {
            nav_target_label.text = nav_target_data->site->name;

            
        }
        NavTargetSharedData new_data = system_list.Update();
        nav_target_data->body = new_data.body;
        nav_target_data->location = new_data.location;
        nav_target_data->site = new_data.site;

    }

    else if(state == MINIMIZED) {

        if(nav_target_data->site) {
            nav_target_label.text = nav_target_data->site->name;
        }
        else if(nav_target_data->location) {
            nav_target_label.text = nav_target_data->location->name;
        }
        else if(nav_target_data->body) {
            nav_target_label.text = nav_target_data->body->name;
        }
        else {
            nav_target_label.text = "N/A";
        }
    }
    top_panel.Update();

}

void Navigation::Draw() {
        
    Rectangle bounds = min_bounds;
    Color color = RED;
    if(state == FOCUSED) {
        color = GREEN;
        bounds = max_bounds;
    }
    DrawRectangleRounded(bounds, 0.2f, 2, TRANSDARKERGRAY);
    DrawRectangleRoundedLines(bounds, 0.2f, 2, color);

    top_label.position = {(bounds.width/2) + bounds.x, bounds.y + 20};
    DrawLabelCentered(top_label, g_font);

    nav_target_label.position = {(bounds.width/2) + bounds.x, bounds.y + 40};
    DrawLabelCentered(nav_target_label, g_font);

    if(state == FOCUSED) {
        Rectangle sys_rec = {
            .x = max_bounds.x + 5,
            .y = max_bounds.y + 140,
            .width = 300.0f,
            .height = 240.0f
        };
        DrawRectangleRec(sys_rec, DARKBLUE);
        system_list.Draw({sys_rec.x + 10, sys_rec.y + 10});
        top_panel.Draw();
    }

}

void Navigation::CreateSystemList(System *system) {

    system_list.body_list.clear();

    for(auto &body : system->map_data.bodies) {
        ListBody new_body;
        new_body.body = &body.second;
        printf("new body\n");

        for(auto &location : system->map_data.locations) {
            if(location.second.body_uid != body.second.uid) {
                continue;
            }
            ListLocation new_location;
            new_location.location = &location.second;
            printf("    new location\n");
            
            for(auto &site : system->map_data.sites) {
                if(site.second.location_uid != location.second.uid) { 
                    continue;
                }
                ListSite new_site;
                new_site.site = &site.second;
                printf("        new site\n\n");
                new_location.site_list.push_back(new_site);

            }

            new_body.location_list.push_back(new_location);
        }

        system_list.body_list.push_back(new_body);
        
    }
    system_list.new_list = true;

}


void Navigation::OnTopPanelButtonPressed() {
    printf("payload = %i   %s\n", top_panel_payload.payload_i, top_panel_payload.payload_s.c_str());

    if(top_panel_payload.payload_s == "ALL") {
        system_list.list_type = SystemList::LIST_TYPE::ALL;
    }
    else if(top_panel_payload.payload_s == "BODIES") {
        system_list.list_type = SystemList::LIST_TYPE::BODIES;
    }
    else if(top_panel_payload.payload_s == "LOCATIONS") {
        system_list.list_type = SystemList::LIST_TYPE::LOCATIONS;
    }
    else if(top_panel_payload.payload_s == "SITES") {
        system_list.list_type = SystemList::LIST_TYPE::SITES;
    }
    system_list.new_list = true;
}