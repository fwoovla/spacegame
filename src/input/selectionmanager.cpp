#include "selectionmanager.hpp"
#include "../game.h"
#include <raymath.h>



SelectionManager::SelectionManager() {

}

void SelectionManager::Update() {
    Vector2 mouse_pos = g_input.world_mouse_position;

    AreaResult result = GetSelection(mouse_pos);

    MouseTriggerArea* new_hover = result.area;

    if(new_hover != current_hover)
    {
        if(current_hover)
        {
            current_hover->mouse_hovering = false;
            current_hover->selected = false;
            current_hover->mouse_exited.EmitSignal();
        }

        current_hover = new_hover;

        if(current_hover) {
            //printf("area %i\n", current_hover->landing_site_payload);
            //printf("hovered area:---- body:%i  location:%i  site:%i\n", current_hover->body_payload, current_hover->location_payload, current_hover->landing_site_payload);
            g_game_data.transition.body_id = current_hover->body_payload;
            g_game_data.transition.location_id = current_hover->location_payload;
            g_game_data.transition.site_id = current_hover->landing_site_payload;

            current_hover->mouse_hovering = true;
            current_hover->mouse_entered.EmitSignal();
        }
    }
    if(current_hover) {

        if(g_input.mouse_left) {
            /*  g_game_data.transition.body_id = current_hover->body_payload;
            g_game_data.transition.location_id = current_hover->location_payload;
            g_game_data.transition.site_id = current_hover->landing_site_payload; */
            current_hover->mouse_triggered.EmitSignal();
            current_hover->selected = true;
            printf("hovered area clicked:---- body:%i  location:%i  site:%i\n", current_hover->body_payload, current_hover->location_payload, current_hover->landing_site_payload);
        }
    }
}


AreaResult SelectionManager::GetSelection(Vector2 mouse_pos) {
    AreaResult result;

    for(MouseTriggerArea* area : areas) {

        if(area == nullptr)
            continue;

        if(area->shape == MouseTriggerArea::CIRCLE)
        {
            float dist = Vector2Distance(mouse_pos, area->position);

            if(dist < area->radius)
            {
                // higher priority wins
                if(area->priority > result.priority ||
                   (area->priority == result.priority && dist < result.distance))
                {
                    result.area = area;
                    result.distance = dist;
                    result.priority = area->priority;
                }
            }
        }
    }

    return result;
}




void SelectionManager::Register(MouseTriggerArea *area) {
    areas.push_back(area);
    printf("registering area:---- body:%i  location:%i  site:%i\n", area->body_payload, area->location_payload, area->landing_site_payload);
}

void SelectionManager::Unregister(MouseTriggerArea *area) {
    std::erase(areas, area);
}