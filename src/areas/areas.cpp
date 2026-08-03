#include "areas.hpp"
#include "../game.h"

/////TRANSITION AREA
TransitionArea::~TransitionArea() {
    //TraceLog(LOG_INFO, "TransitionArea AREA DESTROYED");
}

void TransitionArea::Update() {
    hovered = false;
    
    Rectangle area_rect = {
        .x = position.x,
        .y = position.y,
        .width = (float)size.x,
        .height = (float)size.y
    };

    if(CheckCollisionPointRec( g_input.world_mouse_position, area_rect) ) {
        hovered = true;
        
        in_range = false;

        float lx = g_input.screen_mouse_position.x * g_viewport.inv_scale;
        float ly = (g_input.screen_mouse_position.y - 50) * g_viewport.inv_scale;

        //CreateLabel(label, {lx, ly}, 30, WHITE, "open");

        if(CheckCollisionCircleRec(g_current_player->entity_data->position, 100, area_rect) ) {
            if(type == LANDING) {
                g_game_data.transition.return_position = position;
                g_game_data.transition.location_id = location_uid;
                g_game_data.transition.body_id = body_id;
                g_game_data.transition.system_id = system_id;

            }
            in_range = true;
        }
    }
    else {
        hovered = false;

    }
    if(hovered and g_input.key_use and in_range) {
        time_pressed += 1.0f * GetFrameTime();
        if(time_pressed > 1.0f) {
            time_pressed = 1.0f;
            area_activated.EmitSignal();
        }
    }
    else {
        time_pressed = 0.0f;
    }
    //printf("area update %0.5f %0.5f\n", area_rect.x, area_rect.y);
}



bool TransitionArea::Contains(Vector2 mouse_pos)
{
    switch(shape)
    {
        case CIRCLE:
            return CheckCollisionPointCircle(mouse_pos, position, radius);

        case RECT:
            return CheckCollisionPointRec(mouse_pos, {
                position.x,
                position.y,
                size.x,
                size.y
            });
    }

    return false;
}




//-----------------MOUSE TRIGGER AREA------------------------------
//used to detect mouse-over. nothing else





MouseTriggerArea::~MouseTriggerArea() {
    //TraceLog(LOG_INFO, "MouseTriggerArea AREA DESTROYED");
}

void MouseTriggerArea::Update() {
    hovered = false;

    if(shape == RECT) {
        Rectangle area_rect = {
            .x = position.x,
            .y = position.y,
            .width = (float)size.x,
            .height = (float)size.y
        };

        if(CheckCollisionPointRec( g_input.world_mouse_position, area_rect) ) {
            hovered = true;
            
            float lx = g_input.screen_mouse_position.x * g_viewport.inv_scale;
            float ly = (g_input.screen_mouse_position.y - 50) * g_viewport.inv_scale;

        }
        else {
            hovered = false;
            mouse_hovering = false;

        }
    }
    else if(shape == CIRCLE) {

        if(CheckCollisionPointCircle( g_input.world_mouse_position, position, radius) ) {
            hovered = true;
            
            float lx = g_input.screen_mouse_position.x * g_viewport.inv_scale;
            float ly = (g_input.screen_mouse_position.y - 50) * g_viewport.inv_scale;

        }
        else {
            hovered = false;
            mouse_hovering = false;
            mouse_exited.EmitSignal();
        }


    }
    if(hovered and !mouse_hovering) {
        g_game_data.transition.location_id = location_id;
        mouse_hovering = true;
        mouse_entered.EmitSignal();
    }
}




bool MouseTriggerArea::Contains(Vector2 mouse_pos)
{
    switch(shape)
    {
        case CIRCLE:
            return CheckCollisionPointCircle(mouse_pos, position, radius);

        case RECT:
            return CheckCollisionPointRec(mouse_pos, {
                position.x,
                position.y,
                size.x,
                size.y
            });
    }

    return false;
}












