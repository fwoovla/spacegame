#include "flightcontrol.hpp"
#include "../../game.h"



SystemList::SystemList(Vector2 list_positon) {
    position = list_positon;
}


NavTargetSharedData SystemList::Update() {

    NavTargetSharedData nav_target;
    if(new_list) {
        label_list.clear();
        new_list = false;
        list_size = 0;
        index = 0;
        scroll_index = 0;
        if(list_type == ALL) {
            MakeListAll();
        }
        else if(list_type == BODIES) {
            MakeListBodies();
        }
        else if(list_type == LOCATIONS) {
            MakeListLocations();
        }
        else if(list_type == SITES) {
            MakeListSites();
        }
    }

    

    if(g_input.keys_pressed[0] == KEY_DOWN) {
        index++;
        if(index >= list_size) {
            index = list_size - 1;
        }
    }
    if(g_input.keys_pressed[0] == KEY_UP) {
        index--;
        if(index < 0) {
            index = 0;
        }
    }

    if(index < scroll_index) { scroll_index = index; }

    if(index >= scroll_index + visible_count) { scroll_index = index - visible_count + 1; }

/*     if(g_input.keys_pressed[0] == KEY_HOME) {
        list_type = ALL;
        new_list = true;
    }
    if(g_input.keys_pressed[0] == KEY_DELETE) {
        list_type = BODIES;
        new_list = true;
    }
    if(g_input.keys_pressed[0] == KEY_END) {
        list_type = LOCATIONS;
        new_list = true;
    }
    if(g_input.keys_pressed[0] == KEY_PAGE_DOWN) {
        list_type = SITES;
        new_list = true;
    }
 */
    return nav_target;
}

void SystemList::Draw(Vector2 list_position) {

    int pos_index = 0;
    for(int label_index = scroll_index; label_index <= scroll_index + visible_count; label_index++) {
        Color bg_color = DARKERGRAY;
        if(label_index == index) {
            bg_color = DARKGRAY;
        }
        if(label_index >= label_list.size()) {
            break;
        }
        label_list[label_index].position = {list_position.x, list_position.y + (pos_index * 25)};
        DrawLabelWithBG(label_list[label_index], g_font, bg_color);
        pos_index++;
       
    }
}



void SystemList::MakeListAll() {
    for(auto &body : body_list) {
        Label new_body_label;
        CreateLabel(new_body_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, body.body->name);
        label_list.push_back(new_body_label);
        list_size++;
        for(auto &location : body.location_list) {
            Label new_location_label;
            CreateLabel(new_location_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, location.location->name);
            label_list.push_back(new_location_label);
            list_size++;
            for(auto &site : location.site_list) {
                Label new_site_label;
                CreateLabel(new_site_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, site.site->name);
                label_list.push_back(new_site_label);
                list_size++;
            }
        }
    }
}

void SystemList::MakeListBodies() {
    for(auto &body : body_list) {
        Label new_body_label;
        CreateLabel(new_body_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, body.body->name);
        label_list.push_back(new_body_label);
        list_size++;
    }

}

void SystemList::MakeListLocations() {
for(auto &body : body_list) {
        for(auto &location : body.location_list) {
            Label new_location_label;
            CreateLabel(new_location_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, location.location->name);
            label_list.push_back(new_location_label);
            list_size++;
            
        }
    }

}

void SystemList::MakeListSites() {

    for(auto &body : body_list) {
        for(auto &location : body.location_list) {
            for(auto &site : location.site_list) {
                Label new_site_label;
                CreateLabel(new_site_label, {position.x, position.y + (list_size * 25)}, 24, WHITE, site.site->name);
                label_list.push_back(new_site_label);
                list_size++;
            }
        }
    }
}