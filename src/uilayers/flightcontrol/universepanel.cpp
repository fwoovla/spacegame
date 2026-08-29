#include "flightcontrol.hpp"
#include "../../game.h"

UniversePanel::UniversePanel(Rectangle _bounds) {
    bounds = _bounds;
    center = {
        .x = bounds.x + (bounds.width/2),
        .y = bounds.y + (bounds.height/2)
    };

    map_frame = {
        .x = center.x - (bounds.y + bounds.height - 110)/2,
        .y = center.y - (bounds.y + bounds.height - 110)/2,
        .width = bounds.y + bounds.height - 110,
        .height = bounds.y + bounds.height - 110
    };

    list_frame = {
        .x = bounds.x + 20,
        .y = map_frame.y,
        .width = 250,
        .height = bounds.y + bounds.height - 175
    };

    CreateButton(close_button, {bounds.x + (bounds.width/2), bounds.y + bounds.height - 25}, {100, 30}, GREEN, "CLOSE");
    CreateButton(set_target_button, {list_frame.x + (list_frame.width/2), list_frame.y + list_frame.height + 90}, {100, 30}, GREEN, "SET");

    universe_list = UniverseList(list_frame);

    universe_map = UniverseMap(map_frame);

}

void UniversePanel::Draw() {
    DrawRectangleRounded(bounds, 0.2f, 10, BLACK);

    //DrawRectangleRoundedLines(map_frame, 0.02f, 10, WHITE);
    //DrawRectangleRoundedLines(list_frame, 0.02f, 10, WHITE);

    universe_list.Draw(true);
    universe_map.Draw();
    DrawButton(close_button);

    if(selected_system_data.system != nullptr) {
        if(current_system_uid != selected_system_data.system->uid) {
            DrawButton(set_target_button);
        }
    }

}

void UniversePanel::Update() {

    universe_list.Update(true);
    universe_map.Update();

    if(IsButtonHovered(close_button, g_viewport.scale) and g_input.mouse_left_down) {
        close_universe_panel.EmitSignal();
    }

    if(selected_system_data.system != nullptr) {
        if(current_system_uid != selected_system_data.system->uid) {
            if(IsButtonHovered(set_target_button, g_viewport.scale) and g_input.mouse_left_down) {
                set_system_target.EmitSignal();
            }
        }
    }

}


void UniversePanel::CreateUniverseList(std::unordered_map<int, SystemMapData> *_map_data, int system_uid) {

    universe_list.master_system_list.clear();

    current_system_uid = system_uid;
    universe_map.current_system_uid = system_uid;
    universe_list.current_system_uid = system_uid;

    map_data = _map_data;

    for(auto &[uid, system] : *map_data) {
        UniverseListEntry new_system;
        new_system.system = &system;
        new_system.position = system.map_position / 500;
        CreateLabel(new_system.list_label, {0,0}, 24, RAYWHITE, system.name);
        CreateLabel(new_system.map_label, {0,0}, 24, RAYWHITE, system.name);
        new_system.selected = false;
        universe_list.master_system_list.push_back(new_system);
    }

    universe_list.new_list = true;
}
