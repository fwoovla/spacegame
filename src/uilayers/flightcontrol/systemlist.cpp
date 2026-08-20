#include "flightcontrol.hpp"
#include "../../game.h"



SystemList::SystemList(Vector2 list_positon) {
    position = list_positon;
}


void SystemList::Update() {

    if(shared_nav_data == nullptr) {
        printf("no shared data!!\n");
        return;
    }

    NavTargetSharedData nav_target;
    if(new_list) {
        display_nav_list.clear();
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

    if(g_input.keys_pressed[0] == KEY_ENTER) {
        NavListEntry *entry = &display_nav_list[index];

        printf("!%s selected!\n",entry->label.text.c_str());

        deselect_nav_target.EmitSignal();
        Select(*entry);
    }


}

void SystemList::Draw(Vector2 list_position) {

    int pos_index = 0;
    for(int label_index = scroll_index; label_index <= scroll_index + visible_count; label_index++) {
        Color bg_color = DARKERGRAY;
        if(label_index == index) {
            bg_color = DARKGRAY;
        }
        if(label_index >= display_nav_list.size()) {
            break;
        }
        display_nav_list[label_index].label.position  = {list_position.x, list_position.y + (pos_index * 25)};
        DrawLabelWithBG(display_nav_list[label_index].label, g_font, bg_color);
        pos_index++;
    }
}



void SystemList::MakeListAll() {
    for(auto &entry : master_nav_list) {
        if(entry.body != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
        if(entry.location != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
        if(entry.site != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
    }
}

void SystemList::MakeListBodies() {
    for(auto &entry : master_nav_list) {
        if(entry.body != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
    }

}

void SystemList::MakeListLocations() {
    for(auto &entry : master_nav_list) {
        if(entry.location != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
    }

}

void SystemList::MakeListSites() {
    for(auto &entry : master_nav_list) {
        if(entry.site != nullptr) {
            display_nav_list.push_back(entry);
            list_size++;
        }
    }
}


void SystemList::Select(NavListEntry &entry) {

    if(entry.body != nullptr) {
        shared_nav_data->body = entry.body;
        shared_nav_data->set = true;
        shared_nav_data->body->body_instance->info_area.selected = true;
        shared_nav_data->location = nullptr;
        shared_nav_data->site = nullptr;
    }
    else if(entry.location != nullptr) {
        shared_nav_data->body = nullptr;
        shared_nav_data->location = entry.location;
        shared_nav_data->set = true;
        shared_nav_data->location->location_instance->info_area.selected = true;
        shared_nav_data->site = nullptr;
    }
    else if(entry.site != nullptr) {
        shared_nav_data->body = nullptr;
        shared_nav_data->location = nullptr;
        shared_nav_data->site = entry.site;
        shared_nav_data->set = true;
        shared_nav_data->site->site_instance->info_area.selected = true;
    }
    

}

