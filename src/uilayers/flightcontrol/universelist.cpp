#include "flightcontrol.hpp"
#include "../../game.h"



UniverseList::UniverseList(Rectangle _bounds) {
    bounds = _bounds;
}


void UniverseList::Update(bool focussed) {

    //printf("sys list update??\n");

/*     if(selected_system_data == nullptr) {
        printf("no shared data!!\n");
        return;
    } */
    //NavTargetSharedData nav_target;


    if(new_list) {
        display_system_list.clear();
        new_list = false;
        list_size = 0;
        index = 0;
        scroll_index = 0;
        if(list_type == ALL) {
            MakeListAll();
        }


        visible_count = 15;
        if(display_system_list.size() < 15) {
            visible_count = display_system_list.size();
        }
        printf("sys list new!! %i \n", display_system_list.size());
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
        UniverseListEntry *entry = &display_system_list[index];

        //printf("!%s selected!\n",entry->label.text.c_str());

        Select(*entry);
        select_item.EmitSignal();
        printf("system map position %0.4f, %0.4f\n", selected_system_data->system->map_position.x, selected_system_data->system->map_position.x);
    }
    //printf("sys list update!!\n");
}

void UniverseList::Draw(bool focussed) {
    //printf("sys list draw??\n");

    if(selected_system_data == nullptr) {
        return;
    }

    DrawRectangleRoundedLines(bounds, 0.05f, 10, WHITE);

    int pos_index = 0;
    for(int label_index = scroll_index; label_index < scroll_index + visible_count; label_index++) {
        Label *label = &display_system_list[label_index].list_label;
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

        else if(display_system_list[label_index].selected and focussed) {
            bg_color = GRAY;
            label->default_color = GREEN;
        }

        else if(display_system_list[label_index].selected and !focussed) {
            bg_color = GRAY;
            label->default_color = GREEN;
        }
        

        if(selected_system_data->system != nullptr) {
            if(display_system_list[label_index].system->uid == g_game_data.transition.system_id) {
                //bg_color = GRAY;
                label->default_color = YELLOW;
            }
        }

        if(display_system_list[label_index].system->uid == current_system_uid) {
            label->default_color = BLUE;
        }

        if(label_index >= display_system_list.size()) {
            break;
        }
        label->position  = {bounds.x + 10, (bounds.y + 20) + (pos_index * 25)};
        DrawRectangle(label->position.x - 10, label->position.y, bounds.width, 20, bg_color);
        DrawLabel(*label, g_font);
        pos_index++;
    }
}



void UniverseList::MakeListAll() {
    for(auto &entry : master_system_list) {
        if(entry.system != nullptr) {
            display_system_list.push_back(entry);
            list_size++;

            if(entry.system->uid == current_system_uid) {
                Select(entry);
                index = list_size - 1;
            }
        }
    }

}


void UniverseList::Select(UniverseListEntry &entry) {


    printf("Select entry: %p\n", (void*)&entry);
    printf("  system: %p\n", (void*)entry.system);


    for(auto &_entry : display_system_list) {
        _entry.selected = false;
    }

    if(entry.system != nullptr) {

        entry.selected = true;
        *selected_system_data = entry;
        //selected_system_data = &entry;
        printf("selected entry: %p\n", (void*)selected_system_data);
        printf("selected system: %p\n", (void*)selected_system_data->system);
        printf("system name: %s\n", selected_system_data->system->name.c_str());
    }
}

