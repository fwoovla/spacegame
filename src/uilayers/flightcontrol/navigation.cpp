#include "flightcontrol.hpp"
#include "../../game.h"


Navigation::Navigation(Rectangle min, Rectangle max) {

    min_bounds = min;
    max_bounds = max;

    system_list = SystemList({(max_bounds.width/2) + max_bounds.x, max_bounds.y + 150});
    

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "navigation");
    CreateLabel(nav_target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");
    CreateLabel(nav_distance_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 60}, 30, RAYWHITE, "distance");
    
    Button all_button;// = CreateButton(position, size, color, text)
    CreateButton(all_button, {(max_bounds.width/2) + max_bounds.x, max_bounds.y + 90}, {20, 30}, ORANGE,"A");
    all_button.text_color_focus = DARKERGRAY;
    all_button.text_color = GRAY;
    all_button.default_color = DARKRED;
    top_panel.AddButton(all_button, KEY_HOME, "ALL");

    Button planet_button;// = CreateButton(position, size, color, text)
    CreateButton(planet_button, {(max_bounds.width/2) + max_bounds.x - 50, max_bounds.y + 115}, {20, 30}, ORANGE,"P");
    planet_button.text_color_focus = DARKERGRAY;
    planet_button.text_color = GRAY;
    planet_button.default_color = DARKRED;
    top_panel.AddButton(planet_button, KEY_DELETE, "BODIES");

    Button location_button;// = CreateButton(position, size, color, text)
    CreateButton(location_button, {(max_bounds.width/2) + max_bounds.x, max_bounds.y + 115}, {20, 30}, ORANGE,"L");
    location_button.text_color_focus = DARKERGRAY;
    location_button.text_color = GRAY;
    location_button.default_color = DARKRED;
    top_panel.AddButton(location_button, KEY_END, "LOCATIONS");

    Button site_button;// = CreateButton(position, size, color, text)
    CreateButton(site_button, {(max_bounds.width/2) + max_bounds.x + 50, max_bounds.y + 115}, {20, 30}, ORANGE,"S");
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
    if(shared_nav_data == nullptr) {
        return;
    }


    
    if(state == FOCUSED) {

        top_panel.Update();

        system_list.Update();
        //shared_nav_data->body = new_data.body;
        //shared_nav_data->location = new_data.location;
        //shared_nav_data->site = new_data.site;
    }

    else if(state == MINIMIZED) {

        
    }

    if(shared_nav_data->site) {
        nav_target_label.text = shared_nav_data->site->name;
        shared_nav_data->target_position = shared_nav_data->site->position;
    }
    else if(shared_nav_data->location) {
        nav_target_label.text = shared_nav_data->location->name;
        shared_nav_data->target_position = shared_nav_data->location->position;
    }
    else if(shared_nav_data->body) {
        nav_target_label.text = shared_nav_data->body->name;
        shared_nav_data->target_position = shared_nav_data->body->position;
    }
    else {
        nav_target_label.text = "N/A";
    }
    
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
    nav_distance_label.position = {(bounds.width/2) + bounds.x, bounds.y + 60};
    DrawLabelCentered(nav_distance_label, g_font);

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

    //system_list.shared_nav_data = shared_nav_data;

    system_list.master_nav_list.clear();

    for(auto &body : system->map_data.bodies) {
        NavListEntry new_body;
        new_body.body = &body.second;
        CreateLabel(new_body.label, {0,0}, 24, RAYWHITE, body.second.name);
        system_list.master_nav_list.push_back(new_body);
        printf("new body\n");
        
    }

    for(auto &location : system->map_data.locations) {
        NavListEntry new_location;
        new_location.location = &location.second;
        CreateLabel(new_location.label, {0,0}, 24, RAYWHITE, location.second.name);
        system_list.master_nav_list.push_back(new_location);
        printf("    new location\n");
    }

    for(auto &site : system->map_data.sites) {
        NavListEntry new_site;
        new_site.site = &site.second;
        CreateLabel(new_site.label, {0,0}, 24, RAYWHITE, site.second.name);
        system_list.master_nav_list.push_back(new_site);
        printf("        new site\n\n");
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

/* void Navigation::OnNewNavTareget() {

} */