#include "flightcontrol.hpp"


Navigation::Navigation(Rectangle min, Rectangle max) {
    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "navigation");
    CreateLabel(nav_target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");
}

Navigation::~Navigation() {

}

void Navigation::Update() {
    if(nav_target_data == nullptr) {
        return;
    }
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



    

}

void Navigation::CreateSystemList(System *system) {


    for(auto &body : system->map_data.bodies) {
        ListBody new_body;
        printf("new body\n");

        for(auto &location : system->map_data.locations) {
            if(location.second.body_uid != body.second.uid) {
                continue;
            }
            ListLocation new_location;
            printf("    new location\n");
            
            for(auto &site : system->map_data.sites) {
                if(site.second.location_uid != location.second.uid) { 
                    continue;
                }
                ListSite new_site;
                printf("        new site\n\n");
                new_location.site_list.push_back(new_site);

            }

            new_body.location_list.push_back(new_location);
        }

        system_list.body_list.push_back(new_body);
        
    }

}
/* 
void Navigation::SelectSystemObject(MouseTriggerArea *selected_area, System *system) {

    //MouseTriggerArea *selected_area = selection_manager->selection;
        
    if(selected_area->landing_site_payload != -1) {
        site = &system->map_data.sites[selected_area->landing_site_payload];
    }
    else if(selected_area->location_payload != -1) {
        location = &system->map_data.locations[selected_area->location_payload];
    }
    else if(selected_area->body_payload != -1) {
        body = &system->map_data.bodies[selected_area->body_payload];
    }

    system_object_selected.EmitSignal();
} */