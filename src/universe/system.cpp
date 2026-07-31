#include "system.hpp"
#include "../game.h"

void System::GenerateSystem(SystemMapData &map_data) {

    printf("generating system %i with  %i planets\n", map_data.uid, map_data.body_data.size());

    system_data.uid = map_data.uid;

    system_data.radius = map_data.radius;
    system_data.seed = GetRandomValue(100, 100000);

    system_data.star_position = map_data.star_position;    

    //system_data.body_data = map_data.body_data;    
    
    for(auto &body : map_data.body_data) {
        SpawnSystemBody(body.second);
    }

    ResolveBodyParents();

    for(auto &location : map_data.location_data) {
        for(auto &body : system_data.body_list) {
            if(location.second.body_uid == body->body_data->uid) {
                body->GenerateLocation(location.second);
            }
        }
    }

    for(auto &body : system_data.body_list) {
        for(auto &location : body->locations) {
            for(auto &site : location->landing_sites) {
                site->transition_area->area_activated.Connect([&](){OnTransitionClicked();});
            }
        }
    }

}





/* 
void System::GenerateLandingSites() {

    for(auto &body : system_data.body_list) {
        if(body->body_data->body_type != BODY_STAR) {
            if(GetRandomValue(0, 100) > 0) {
                body->body_data->landable = true;

                std::unique_ptr<TransitionSite> new_site = std::make_unique<TransitionSite>();
                new_site->transition_area = std::make_unique<TransitionArea>();
                
                new_site->transition_area->size = {20, 20};
                new_site->position.x = (float)GetRandomValue(-body->body_data->radius, body->body_data->radius) + body->body_data->position.x;
                new_site->transition_area->position.x = new_site->position.x - (new_site->transition_area->size.x * 0.5f);

                new_site->position.y = (float)GetRandomValue(-body->body_data->radius, body->body_data->radius) + body->body_data->position.y;
                new_site->transition_area->position.y = new_site->position.y - (new_site->transition_area->size.y * 0.5f);

                new_site->transition_area->type = LANDING;
                new_site->transition_area->area_activated.Connect([&](){OnTransitionClicked();});
                body->landing_sites.push_back(std::move(new_site));
            }
        }
    }

}

 */




void System::Update() {
    for(auto &body : system_data.body_list) {
        body->Update();
    }

    auto &vec = system_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }

    std::erase_if(vec, [](const std::unique_ptr<BaseEntity> &entity){return entity->should_delete;});
}



void System::Draw() {

    
    for(auto &body : system_data.body_list) {
        body->Draw();
    }

    for(auto &entity : system_data.entity_list) {
        entity->Draw();
    }
}


void System::DrawWorld() {

    
    for(auto &body : system_data.body_list) {
        body->Draw();
    }

    for(auto &entity : system_data.entity_list) {
        if(entity->entity_data->render_mode == RENDER_WORLD)
            entity->Draw();
    }
}

void System::DrawOverlay() {

    for(auto &entity : system_data.entity_list) {
        if(entity->entity_data->render_mode != RENDER_WORLD)
            entity->DrawOverlay();
    }
    for(auto &body : system_data.body_list) {
        body->DrawOverlay();
    }
}


void System::DrawDebug() {


}

void System::DrawUI() {

    for(auto &entity : system_data.entity_list) {
        entity->DrawUI();
    }
    for(auto &body : system_data.body_list) {
        body->DrawUI();
    }
}



void System::OnTransitionClicked() {

    //g_game_data.transition.location_id = ;

    landing_requested.EmitSignal();
}

PlayerCharacter * System::SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData entity_data = GenerateEntityInstance(tmpl, uid, position);
    system_data.entity_data[entity_data.uid] = entity_data;

    std::unique_ptr<PlayerCharacter> player = std::make_unique<PlayerCharacter>(&system_data.entity_data[entity_data.uid]);
    PlayerCharacter * ptr = player.get();
    system_data.entity_list.push_back(std::move(player));

    return ptr;
}

 SystemBodyEntity * System::SpawnSystemBody(SystemBodyData &data) {

    system_data.body_data[data.uid] = data;


    std::unique_ptr<SystemBodyEntity> body = std::make_unique<SystemBodyEntity>(&system_data.body_data[data.uid]);
    SystemBodyEntity * ptr = body.get();

    system_data.body_list.push_back(std::move(body));
    printf("spawning body  type: %i  |uid: %i   |parent uid: %i\n",ptr->body_data->body_type , ptr->body_data->uid, ptr->body_data->parent_body_uid);
    return ptr;

 }


 void System::ResolveBodyParents() {
    for (auto &[uid, body] : system_data.body_data) {
        body.parent = nullptr;

        if (body.parent_body_uid == -1)
            continue;

        auto it = system_data.body_data.find(body.parent_body_uid);

        if (it != system_data.body_data.end())
            body.parent = &it->second;
    }
}