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

    //universe_list.Draw(true);
    universe_map.Draw();
    DrawButton(close_button);

    if(can_jump) {
        DrawButton(set_target_button);
    }    

}

void UniversePanel::Update() {

    universe_list.Update(true);
    universe_map.Update();

    if(IsButtonHovered(close_button, g_viewport.scale) and g_input.mouse_left_down) {
        close_universe_panel.EmitSignal();
    }

    can_jump = false;
    if(selected_system_data.system != nullptr) {
        if(current_system_uid != selected_system_data.system->uid) {

            for(auto &connection : universe->connections) {

                if(connection.system_a_uid == selected_system_data.system->uid or connection.system_b_uid == selected_system_data.system->uid) {
                    if(connection.activated) {
                        can_jump = true;           
                    }
                }
            }
            if(can_jump) {
                if(IsButtonHovered(set_target_button, g_viewport.scale) and g_input.mouse_left_down) {
                    set_system_target.EmitSignal();
                }
            }
        }
    }

}




void UniversePanel::CreateUniverseList(UniverseData *_universe, int system_uid) {

    current_system_uid = system_uid;

    universe = _universe;
    map_data = &universe->map_data;

    
    universe_map.current_system_uid = system_uid;
    universe_map.universe = universe;
    
    universe_list.master_system_list.clear();
    universe_list.current_system_uid = system_uid;


    for(auto &[uid, system] : *map_data) {
        UniverseListEntry new_system;
        new_system.system = &system;
        new_system.position = system.map_position / 500;
        CreateLabel(new_system.list_label, {0,0}, 24, RAYWHITE, system.name);
        CreateLabel(new_system.map_label, {0,0}, 24, RAYWHITE, system.name);
        new_system.selected = false;

/* 
        for(auto &connection : universe->connections) {
            if(connection.system_a_uid == system.uid or connection.system_b_uid == system.uid) {
                Vector2 c_pos;
                if(connection.system_a_uid == system.uid) {
                    c_pos = map_data->at(connection.system_b_uid).map_position;
                    //c_pos = universe->map_data[connection.system_b_uid].map_position;
                }
                else if(connection.system_b_uid == system.uid) {
                    c_pos = map_data->at(connection.system_a_uid).map_position;
                }
                //new_system.master_connection_points.push_back(c_pos);
                //new_system.display_connection_points.push_back(c_pos);
            }
        } */

        universe_list.master_system_list.push_back(new_system);
    }

    universe_list.new_list = true;
}
