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


    for(auto &star : stars) {
        DrawCircleV(star, 1, WHITE);
    }

    DrawButton(close_button);
    
}

void UniverseMap::Update() {

    stars.clear();

    for(auto &[uid, system] : *map_data) {
        Vector2 pos = system.map_position / 1000;
        pos = Vector2Add( {center.x, center.y}, pos);
        stars.push_back(pos);
    }

    if(IsButtonHovered(close_button, g_viewport.scale)) {
        if(g_input.mouse_left_down) {
            close_universe_map.EmitSignal();
        }
    }
}
