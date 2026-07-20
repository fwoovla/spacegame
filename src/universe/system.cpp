#include "system.hpp"
#include "../game.h"

void System::GenerateSystem() {

    system_data.radius = 5000.0f;
    system_data.seed = GetRandomValue(100, 100000);

    system_data.star_position = {system_data.radius, system_data.radius};
    int num_planets = 10; GetRandomValue(1, 10);

    SpawnSystemBody(system_data.star_position, BODY_STAR, num_planets, nullptr);

    GenerateBodies();

    GenerateLandingSites();


}




void System::GenerateBodies() {

{
    int num_planets = 5; //GetRandomValue(1, 10);
    SystemBodyEntity *sun = system_data.body_list[0].get();
    Vector2 sun_pos = sun->body_data->position;
    int p = 0;
    int num_layers = 20;
    float layer_size = system_data.radius/num_layers;

    for(int o_layer = 5; o_layer < num_layers; o_layer++) {

        if(GetRandomValue(0,100) > 0 and p < num_planets) {
            
            int orbitals = GetRandomValue(0, 2);
            SystemBodyEntity *body = SpawnSystemBody({0,0}, BODY_PLANET, orbitals, sun);
            
            body->body_data->orbit_radius = o_layer * layer_size;
            body->body_data->orbit_angle = DEG2RAD * GetRandomValue(0, 359);
            system_data.orbitals.push_back(body->body_data->orbit_radius); 


            Vector2 pos;

            pos.x = sun_pos.x + cosf(body->body_data->orbit_angle) * body->body_data->orbit_radius;
            pos.y = sun_pos.y + sinf(body->body_data->orbit_angle) * body->body_data->orbit_radius;

            body->body_data->position = pos;
            p++;
            printf("planet   %0.5f %0.5f\n", pos.x, pos.y + o_layer);
        }
    }
}
    //orbiting bodies

    int parentCount = system_data.body_list.size();
    for(int planet = 1; planet < parentCount; planet++) {

        SystemBodyEntity *parent_body = system_data.body_list[planet].get();
        Vector2 parent_pos = parent_body->body_data->position;

        int num_bodies = 1;
        int o = 0;
        int num_p_layers = 10;

        float p_layer_size = (parent_body->body_data->radius * 10) / num_p_layers;

        for(int p_layer = 5; p_layer < num_p_layers; p_layer++) {
            if(GetRandomValue(0,100) > 0 and o < num_bodies ) {

                SystemBodyEntity *body = SpawnSystemBody({0,0}, BODY_MOON, 0, parent_body);

                body->body_data->orbit_radius = p_layer * p_layer_size;
                body->body_data->orbit_angle = DEG2RAD * GetRandomValue(0, 359);
                parent_body->body_data->orbitals.push_back(body->body_data->orbit_radius);

                Vector2 pos;

                pos.x = parent_pos.x + cosf(body->body_data->orbit_angle) * body->body_data->orbit_radius;
                pos.y = parent_pos.y + sinf(body->body_data->orbit_angle) * body->body_data->orbit_radius;

                body->body_data->position = pos;
                o++;
                printf("orbiting body   %0.5f %0.5f\n", pos.x, pos.y + p_layer);
            }
        }
    }

}


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


void System::OnTransitionClicked() {

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


void System::Spawnentity(EntityTemplateData &tmpl, int uid, Vector2 position) {


}

EntityData System::GenerateEntityInstance(EntityTemplateData &tmpl, int uid, Vector2 position) {

    EntityData instance_data;


    instance_data.uid = GetUID();
    instance_data.name = tmpl.name;
    instance_data.id = tmpl.id;
    instance_data.obstructable = tmpl.obstructable;
    instance_data.position = position;


    instance_data.component_flags = tmpl.component_flags;

    instance_data.health = tmpl.health;
    instance_data.inventory = tmpl.inventory;
    instance_data.movement = tmpl.movement;
    instance_data.interaction = tmpl.interaction;

    instance_data.collision_rect = {
        position.x,
        position.y,
        tmpl.size.x,
        tmpl.size.y
    };

    instance_data.radius = tmpl.size.x/2;

    return instance_data;
}



SystemBodyEntity * System::SpawnSystemBody(Vector2 position, BODY_TYPE type, int orbitals, SystemBodyEntity *parent_body) {

    SystemBodyData data = GenerateSystemBodyInstance(position, type, orbitals);
    system_data.body_data[data.uid] = data;

    std::unique_ptr<SystemBodyEntity> body = std::make_unique<SystemBodyEntity>(&system_data.body_data[data.uid]);
    SystemBodyEntity * ptr = body.get();

    system_data.body_list.push_back(std::move(body));
    return ptr;

}

SystemBodyData System::GenerateSystemBodyInstance(Vector2 position, BODY_TYPE type, int orbitals) {



    SystemBodyData instance_data;
    instance_data.body_type = type;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.name = "star " + std::to_string(uid);
    instance_data.obstructable = false;
    instance_data.position = position;

    if(type == BODY_STAR) {
        instance_data.modulate = ORANGE;
        instance_data.radius = 1500.0f;
    }
    else if(type == BODY_PLANET) {
        instance_data.modulate = BLUE;
        instance_data.radius = 200.0f;
    }
    else if(type == BODY_MOON) {
        instance_data.modulate = YELLOW;
        instance_data.radius = 50.0f;
    }


    instance_data.orbiting_bodies_count = orbitals;

    return instance_data;
}