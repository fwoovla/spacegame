#include "system.hpp"
#include "../game.h"

System::System( SystemMapData &_map_data ) : map_data(_map_data) {
}

void System::GenerateSystem(SelectionManager *sm) {

    printf("generating system %i with  %i planets\n", map_data.uid, map_data.bodies.size());

    selection_manager = sm;

    system_data.uid = map_data.uid;

    system_data.radius = map_data.radius;
    system_data.seed = GetRandomValue(100, 100000);

    system_data.star_position = map_data.star_position;    

    //system_data.body_data = map_data.body_data;    
    
    for(auto &body : map_data.bodies) {
        SpawnSystemBody(body.second);
    }

    for(auto &location : map_data.locations) {
        SpawnSystemLocation(location.second);
    }

    for(auto &site : map_data.sites) {
        SpawnSystemSite(site.second);
    }

    ResolveParents();


    for(auto &site : system_data.site_list) {
        site->RegisterWithManagers(selection_manager);
    }
    for(auto &location : system_data.location_list) {
        location->RegisterWithManagers(selection_manager);
    }
    for(auto &body : system_data.body_list) {
        body->RegisterWithManagers(selection_manager);
    }
}



void System::Update() {
    for(auto &site : system_data.site_list) {
        site->Update();
    }

    for(auto &location : system_data.location_list) {
        location->Update();
    }

    for(auto &body : system_data.body_list) {
        body->Update();
    }

    auto &vec = system_data.entity_list;

    for (auto &entity : vec) {
        entity->Update();
    }

    std::erase_if(vec, [](const std::unique_ptr<CreatureEntity> &entity){return entity->should_delete;});
}



void System::Draw() {
/* 
    for(auto &site : system_data.site_list) {
        site->Draw();
    }
    
    for(auto &location : system_data.location_list) {
        location->Draw();
    }

    for(auto &body : system_data.body_list) {
        body->Draw();
    }
    

    for(auto &entity : system_data.entity_list) {
        entity->Draw();
    } */
}


void System::DrawWorld() {

    
    
    for(auto &body : system_data.body_list) {
        body->Draw();
        //DrawCircleLinesV(body->body_data->position, body->detect_radius, WHITE);
    }
    for(auto &location : system_data.location_list) {
        location->Draw();
        DrawCircleLinesV(location->location_data->position, location->detect_radius, WHITE);
    }

    for(auto &site : system_data.site_list) {
        site->Draw();
        DrawCircleLinesV(site->site_data->position, site->detect_radius, WHITE);
    }
    
    for(auto &entity : system_data.entity_list) {
        if(entity->entity_data->render_mode == RENDER_WORLD)
            entity->Draw();
    }
}

void System::DrawOverlay() {

    for(auto &body : system_data.body_list) {
        body->DrawOverlay();
    }
    for(auto &location : system_data.location_list) {
        location->DrawOverlay();
    }
    for(auto &site : system_data.site_list) {
        site->DrawOverlay();
    }
    
    
    for(auto &entity : system_data.entity_list) {
        if(entity->entity_data->render_mode != RENDER_WORLD)
            entity->DrawOverlay();
    }
}


void System::DrawDebug() {


}

void System::DrawUI() {

    for(auto &entity : system_data.entity_list) {
        entity->DrawUI();
    }
    for(auto &site : system_data.site_list) {
        site->DrawUI();
    }
    
    for(auto &location : system_data.location_list) {
        location->DrawUI();
    }
    for(auto &body : system_data.body_list) {
        body->DrawUI();
    }
}


PlayerCharacter * System::SpawnPlayer(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData entity_data = GenerateEntityInstance(tmpl, uid, position);
    system_data.entity_data[entity_data.uid] = entity_data;

    std::unique_ptr<PlayerCharacter> player = std::make_unique<PlayerCharacter>(&system_data.entity_data[entity_data.uid]);
    PlayerCharacter * ptr = player.get();
    system_data.entity_list.push_back(std::move(player));

    return ptr;
}

 SystemBody * System::SpawnSystemBody(SystemBodyData &data) {

    std::unique_ptr<SystemBody> body = std::make_unique<SystemBody>(&data);
    SystemBody * ptr = body.get();

    system_data.body_list.push_back(std::move(body));
    printf("spawning body  type: %i  |uid: %i   |parent uid: %i\n",ptr->body_data->body_type , ptr->body_data->uid, ptr->body_data->parent_uid);
    return ptr;

 }



SystemLocation * System::SpawnSystemLocation(SystemLocationData &data) {
    std::unique_ptr<SystemLocation> location = std::make_unique<SystemLocation>(&data);
    SystemLocation * ptr = location.get();

    system_data.location_list.push_back(std::move(location));
    printf("spawning location   |uid: %i   |body uid: %i\n", ptr->location_data->uid, ptr->location_data->body_uid);
    return ptr;
}


SystemSite * System::SpawnSystemSite(SystemSiteData &data) {
    std::unique_ptr<SystemSite> site = std::make_unique<SystemSite>(&data);
    SystemSite * ptr = site.get();

    system_data.site_list.push_back(std::move(site));
    printf("spawning site   |uid: %i   |body uid: %i\n", ptr->site_data->uid, ptr->site_data->body_uid);
    return ptr;
}



 void System::ResolveParents() {

    for (auto &body : system_data.body_list) {
        body->parent_data = {};

        int p_uid = body->body_data->parent_uid;

        if (p_uid == -1)
            continue;

        auto it = map_data.bodies.find(p_uid);

        if (it != map_data.bodies.end())
            body->parent_data = &it->second;
    }
}