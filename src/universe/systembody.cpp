#include "entity.hpp"
#include "../game.h"

SystemBodyEntity::SystemBodyEntity(SystemBodyData *_data) {
    body_data = _data;

}

void SystemBodyEntity::Update() {
    for(auto &site : landing_sites) {
        site->transition_area->Update();
        //printf("landing site\n");
    }
}

void SystemBodyEntity::Draw() {
    DrawCircleV(body_data->position, body_data->radius, body_data->modulate);

    for(auto &site : landing_sites) {
        //DrawCircle(site->position.x, site->position.y, site->transition_area->size.x, GRAY);
        DrawRectangle(site->transition_area->position.x, site->transition_area->position.y, site->transition_area->size.x, site->transition_area->size.y, RED);
        //printf("landing site\n");
    }
}

void SystemBodyEntity::DrawUI() {

    for(auto &site : landing_sites) {
        site->transition_area->Draw();
    }
}