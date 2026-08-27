#include "flightcontrol.hpp"
#include "../../game.h"

UniverseMap::UniverseMap(Rectangle _bounds) {
    bounds = _bounds;
    center = {
        .x = bounds.x + (bounds.width/2),
        .y = bounds.y + (bounds.height/2)
    };

    universe_frame = {
        .x = center.x - (bounds.y + bounds.height - 110)/2,
        .y = center.y - (bounds.y + bounds.height - 110)/2,
        .width = bounds.y + bounds.height - 110,
        .height = bounds.y + bounds.height - 110
    };

    list_frame = {
        .x = bounds.x + 20,
        .y = universe_frame.y,
        .width = 150,
        .height = bounds.y + bounds.height - 110
    };

    CreateButton(close_button, {bounds.x + (bounds.width/2), bounds.y + bounds.height - 40}, {100, 30}, GREEN, "CLOSE");
}

void UniverseMap::Draw() {
    DrawRectangleRounded(bounds, 0.2f, 10, BLACK);

    DrawRectangleRoundedLines(universe_frame, 0.02f, 10, WHITE);
    DrawRectangleRoundedLines(list_frame, 0.02f, 10, WHITE);


    for(auto &entry : system_entries) {
        DrawCircleV(entry.position, 1, WHITE);
        DrawLabel(entry.list_label, g_font);
        DrawLabel(entry.map_label, g_font);
    }

    DrawButton(close_button);
    
}

void UniverseMap::Update() {

    system_entries.clear();

    int list_index = 0;
    for(auto &[uid, system] : *map_data) {
        UniverseMapEntry new_entry;

        Vector2 pos = system.map_position / 1000;
        pos = Vector2Add( {center.x, center.y}, pos);
        new_entry.position = pos;

        new_entry.system = &system;


        CreateLabel(new_entry.list_label, {list_frame.x, list_frame.y + (30 * list_index)}, 20, RAYWHITE, system.name);
        CreateLabel(new_entry.map_label, pos, 20, RAYWHITE, system.name);


        system_entries.push_back(new_entry);
        list_index++;
    }

    if(IsButtonHovered(close_button, g_viewport.scale)) {
        if(g_input.mouse_left_down) {
            close_universe_map.EmitSignal();
        }
        
    }
}
