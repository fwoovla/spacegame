#include "flightcontrol.hpp"
#include "../../game.h"

UniversePanel::UniversePanel(Rectangle _bounds) {
    bounds = _bounds;
    center = {
        .x = bounds.x + (bounds.width/2),
        .y = bounds.y + (bounds.height/2)
    };

    map_frame = {
        .x = center.x - (bounds.y + bounds.height + 500)/2,
        .y = center.y - (bounds.y + bounds.height - 110)/2,
        .width = bounds.y + bounds.height + 200,
        .height = bounds.y + bounds.height - 110
    };

    list_frame = {
        .x = bounds.x + 20,
        .y = map_frame.y,
        .width = 200,
        .height = bounds.y + bounds.height - 175
    };

    CreateButton(close_button, {bounds.x + (bounds.width/2), bounds.y + bounds.height - 25}, {100, 30}, GREEN, "CLOSE");
    
    CreateLabel(details_header_label, {map_frame.x + map_frame.width + 25, map_frame.y + 10}, 40, WHITE, "DEATILS");
    CreateLabel(details_label, {details_header_label.position.x + 10, details_header_label.position.y + 30}, 24, WHITE, "DEATILS");

    CreateButton(set_target_button, {details_label.position.x + 100, bounds.y + bounds.height - 25}, {100, 30}, GREEN, "SET");

    CreateButton(center_button, {details_header_label.position.x + 240, details_header_label.position.y}, {100, 30}, GREEN, "CENTER");
    center_button.text_size = 24;

    universe_list = UniverseList(list_frame);

    universe_map = UniverseMap(map_frame);

}

void UniversePanel::Draw() {
    DrawRectangleRounded(bounds, 0.2f, 10, BLACK);

    DrawRectangleRoundedLines(map_frame, 0.02f, 10, WHITE);
    //DrawRectangleRoundedLines(list_frame, 0.02f, 10, WHITE);

    //universe_list.Draw(true);
    universe_map.Draw();
    DrawButton(close_button);

    if(can_jump) {
        DrawButton(set_target_button);
    }
    if(!universe_map.use_selected_to_center) {
        DrawButton(center_button);
    }

    DrawLine(details_header_label.position.x - 20, 
        details_header_label.position.y - 10, 
        details_header_label.position.x + 150, 
        details_header_label.position.y - 10, 
        WHITE
    );

    DrawLabel(details_header_label, g_font);
    DrawLabel(details_label, g_font);

}

void UniversePanel::Update() {

    universe_list.Update(false);
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
    if(selected_system_data.system != nullptr) {

        if(!universe_map.use_selected_to_center) {
            if(IsButtonHovered(center_button, g_viewport.scale) and g_input.mouse_left_down) {
                    universe_map.use_selected_to_center = true;
                }
        }

        details_label.default_color = RED;

        details_label.text = "name: unexplored";
        details_label.text += "\nbodies: ?";
        details_label.text += "\nlocations: ?";
        details_label.text += "\nlanding sites: ?";
        
        if(selected_system_data.system->examined) {
            details_label.default_color = GOLD;
            details_label.text = "name: " + selected_system_data.system->name;
            details_label.text += "\nbodies: " + std::to_string(selected_system_data.system->bodies.size());
            details_label.text += "\nlocations: ?";
            details_label.text += "\nlanding sites: ?";
        }

        if(selected_system_data.system->discovered) {
            details_label.default_color = GREEN;
            details_label.text = "name: " + selected_system_data.system->name;
            details_label.text += "\nbodies: " + std::to_string(selected_system_data.system->bodies.size());
            details_label.text += "\nlocations: " + std::to_string(selected_system_data.system->locations.size());
            details_label.text += "\nlanding sites: " + std::to_string(selected_system_data.system->sites.size());
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
