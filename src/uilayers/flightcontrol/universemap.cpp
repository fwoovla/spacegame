#include "flightcontrol.hpp"
#include "../../game.h"


UniverseMap::UniverseMap(Rectangle _bounds) {
    bounds = _bounds;
    center = {
        .x = bounds.x + (bounds.width/2),
        .y = bounds.y + (bounds.height/2)
    };

}

void UniverseMap::Draw() {

    //DrawRectangleRounded(bounds, 0.2f, 10, BLACK);
    //DrawRectangleRoundedLines(bounds, 0.02f, 10, WHITE);

    for(UniverseListEntry &entry : *display_system_list) {
        Vector2 map_pos = Vector2Add(entry.position, center);

        if(!CheckCollisionPointRec(map_pos, bounds)) {
            continue;
        }


        entry.map_label.position = Vector2Add(map_pos, (Vector2{0, -10}));
        
        Color color = WHITE;

        if(entry.selected) {
            //color = GREEN;
            DrawCircleLinesV(map_pos, 12, GREEN);
        }

        //entry.map_label.default_color = color;
        DrawLabelCentered(entry.map_label, g_font);
        DrawCircleV(map_pos, 3, WHITE);

        if(entry.system->uid == current_system_uid) {
            DrawCircleLinesV(map_pos, 10, BLUE);
        }

        if(selected_system_data->system != nullptr) {
            if(entry.system->uid == g_game_data.transition.system_id) {
                DrawCircleLinesV(map_pos, 14 + (5 * animation_factor), YELLOW);
            }
        }

        if(CheckCollisionPointCircle(g_input.screen_mouse_position, map_pos, 10)) {
            DrawCircleV(map_pos, 10, WHITE);
            if(g_input.mouse_left) {
                selected_system_data->selected = false;

                for(auto &e : *display_system_list) {
                    e.selected = false;
                }

                entry.selected = true;
                *selected_system_data = entry;
            }

        }
    }
}

void UniverseMap::Update() {
    animation_factor = sin( GetTime() * 3 );
    //printf("animation : %0.5f\n", animation_factor);

    if(selected_system_data == nullptr) {
        return;
    }
    
    HandleMapMovement();

}


void UniverseMap::HandleMapMovement() {

    float wheel_zoom = g_input.mouse_wheel * 20.0f;
    
    map_scale -= wheel_zoom;
    if(map_scale < 50.0f) {
        map_scale = 50.0f;
    }
    if(map_scale > 2000.0f) {
        map_scale = 2000.0f;
    }

    if(selected_system_data->system == nullptr) {
        return;
    }

    Vector2 offset = selected_system_data->system->map_position / map_scale;

    for(auto &entry : *display_system_list) {
        Vector2 map_pos = entry.system->map_position / map_scale;
        Vector2 target_pos = Vector2Subtract(map_pos, offset);
        entry.position = Vector2Lerp(entry.position, target_pos, 0.1f);
    }
}