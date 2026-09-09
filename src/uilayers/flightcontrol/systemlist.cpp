#include "flightcontrol.hpp"
#include "../../game.h"



SystemList::SystemList(Vector2 list_positon) {
    position = list_positon;
}


void SystemList::Update(bool focussed) {

    //printf("sys list update??\n");

    if(shared_list_data == nullptr) {
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


        if(display_nav_list.size() < 8) {
            visible_count = display_nav_list.size();
        }
        else {
            visible_count = 8;
        }
        //printf("sys list new!!\n");
    }

    

    if(!focussed) {
        return;
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

        //printf("!%s selected!\n",entry->label.text.c_str());

        Select(*entry);
        select_item.EmitSignal();
    }
    //printf("sys list update!!\n");
}

void SystemList::Draw(Vector2 list_position, bool focussed) {
    //printf("sys list draw??\n");

    int pos_index = 0;
    for(int label_index = scroll_index; label_index < scroll_index + visible_count; label_index++) {
        Label *label = &display_nav_list[label_index].label;
        //printf("sys list draw index %i ??\n", label_index);
        Color bg_color = BLACK;
        label->default_color = RAYWHITE;

        if(label_index == index and focussed) {
            bg_color = ORANGE;
            label->default_color = DARKERGRAY;
        }
        else if(label_index == index and !focussed) {
            bg_color = DARKGRAY;
            label->default_color = RAYWHITE;
        }
        if(label_index >= display_nav_list.size()) {
            break;
        }
        label->position  = {list_position.x, list_position.y + (pos_index * 25)};
        DrawRectangle(list_position.x - 20, label->position.y, list_position.x + 150, 20, bg_color);
        DrawLabel(*label, g_font);
        pos_index++;
        //printf("sys list draw index %i \n", label_index);
    }
    //printf("sys list draw!!\n");
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
        shared_list_data->body = entry.body;
        shared_list_data->set = true;
        //shared_list_data->body->body_instance->info_area.selected = true;
        shared_list_data->location = nullptr;
        shared_list_data->site = nullptr;
    }
    else if(entry.location != nullptr) {
        shared_list_data->body = nullptr;
        shared_list_data->location = entry.location;
        shared_list_data->set = true;
        //shared_list_data->location->location_instance->info_area.selected = true;
        shared_list_data->site = nullptr;
    }
    else if(entry.site != nullptr) {
        shared_list_data->body = nullptr;
        shared_list_data->location = nullptr;
        shared_list_data->site = entry.site;
        shared_list_data->set = true;
        //shared_list_data->site->site_instance->info_area.selected = true;
    }
}

