#include "universe.hpp"

void UniverseGen_MakeSystem(SystemMapData &sys_map_data) {
    printf("------------------- System: %s -------------------\n", sys_map_data.name.c_str());
    SystemBodyData star_body_data = GenerateSystemStarData(sys_map_data);
    sys_map_data.bodies[star_body_data.uid] = star_body_data;

    int p_tally = 0;
    for(int o_layer = 6; o_layer < star_body_data.orbital_layer_count; o_layer++) {
        if(GetRandomValue(0,100) > 75 and p_tally < star_body_data.orbital_body_count) {                
            SystemBodyData body_data = GenerateSystemBodyData(BODY_PLANET, 
                o_layer, 
                star_body_data.orbital_layer_delta, 
                &star_body_data);
            p_tally++;
            sys_map_data.bodies[body_data.uid] = body_data;
            star_body_data.orbital_body_uids.push_back(body_data.uid);
        }
    }

    std::vector<SystemBodyData*> planets;
    for (auto &[uid, body] : sys_map_data.bodies) {
        if (body.body_type == BODY_PLANET)
            planets.push_back(&body);
    }

    for (SystemBodyData *planet : planets) {
        int m_tally = 0;
        for(int o_layer = 5; o_layer < planet->orbital_layer_count; o_layer++) {
            if(GetRandomValue(0,100) > 75 and m_tally < planet->orbital_body_count) {               
                SystemBodyData moon_data = GenerateSystemBodyData(BODY_MOON, o_layer, planet->orbital_layer_delta, planet);
                sys_map_data.bodies[moon_data.uid] = moon_data;
                planet->orbital_body_uids.push_back(moon_data.uid);
                m_tally++;
            }
        }
    }

    UniverseGen_MakeLocations(sys_map_data);
    UniverseGen_MakeSites(sys_map_data);
    printf("------------------- END SYSTEM -------------------\n\n");
}

void UniverseGen_MakeLocations(SystemMapData &sys_map_data) {
    //printf("------------------- Location on Body: %s -------------------\n", sys_map_data.name.c_str());
    std::vector<SystemBodyData*> bodies;

    for (auto &[uid, body] : sys_map_data.bodies) {
            if (body.landable == true)
                bodies.push_back(&body);
                //printf("location added --- name: %s\n", body.name.c_str());
        }

    int s = 1;
    for (SystemBodyData *body : bodies) {

        printf(" Location on Body: %s -------------------\n", body->name.c_str());
        SystemLocationData new_location = GenerateSystemLocationData(body);
        sys_map_data.locations[new_location.uid] = new_location;
        printf(" END Location -------------------\n");
    }

}


void UniverseGen_MakeSites(SystemMapData &sys_map_data) {

    std::vector<SystemLocationData*> locations;

    for (auto &[uid, location] : sys_map_data.locations) {
        locations.push_back(&location);
        //printf("locations added name: %s\n", body.name.c_str());
    }
    for (SystemLocationData *location : locations) {
        for(int uid : location->local_data.site_uids) {
            printf(" Site at Location: %s -------------------\n", location->name.c_str());
            SystemSiteData new_site = GenerateSystemSiteData(location, uid);
            sys_map_data.sites[new_site.uid] = new_site;
            location->site_uids.push_back(new_site.uid);
            printf(" END Site -------------------\n");
        }
    }

}











SystemBodyData GenerateSystemStarData(SystemMapData &map_data) {


    SystemBodyData instance_data;
    instance_data.body_type = BODY_STAR;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.obstructable = false;
    instance_data.landable = false;
    instance_data.position = map_data.star_position;
    instance_data.parent_uid = uid;


    instance_data.name = "star " + std::to_string(uid);
    instance_data.modulate = ORANGE;
    instance_data.radius = 8000.0f;

    instance_data.orbital_body_count = GetRandomValue(1,10);
    instance_data.orbital_layer_count = GetRandomValue(10, 20);
    instance_data.orbital_layer_delta = map_data.radius / instance_data.orbital_layer_count;

    
    printf("star  %0.5f %0.5f  delta %0.5f\n", instance_data.position.x, instance_data.position.y, instance_data.orbital_layer_delta);
    return instance_data;

}

/*  */
SystemBodyData GenerateSystemBodyData( BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent) {

    SystemBodyData instance_data;
    instance_data.body_type = type;
    int uid = GetUID();
    instance_data.uid = uid;
    instance_data.obstructable = false;
    instance_data.landable = true;
    instance_data.position = {0,0};
    instance_data.parent_uid = parent->uid;

    if(type == BODY_PLANET) {
        instance_data.name = "planet " + std::to_string(uid);
        instance_data.modulate = g_planet_colors[GetRandomValue(0, g_planet_colors.size() - 1)];
        instance_data.radius = 4000.0f;
        instance_data.orbital_body_count = GetRandomValue(0,5);
        instance_data.orbital_layer_count = GetRandomValue(5, 20);
        instance_data.orbital_layer_delta = (instance_data.radius * 5) /instance_data.orbital_layer_count;
        
    }
    else if(type == BODY_MOON) {
        instance_data.name = "moon " + std::to_string(uid);
        instance_data.modulate = g_planet_colors[GetRandomValue(0, g_planet_colors.size() - 1)];
        instance_data.radius = 1800.0f;
        instance_data.orbital_body_count = 0;
    }

    instance_data.orbit_radius = layer * layer_delta;
    instance_data.orbit_angle = DEG2RAD * GetRandomValue(0, 359);           

    
    if(parent != nullptr) {

        Vector2 pos = {0,0};
        pos.x = parent->position.x + cosf(instance_data.orbit_angle) * instance_data.orbit_radius;
        pos.y = parent->position.y + sinf(instance_data.orbit_angle) * instance_data.orbit_radius;
        instance_data.position = pos;

        instance_data.parent_orbital = layer;
    }

    printf("body: %i  uid: %i data created  %0.5f %0.5f  delta %0.5f\n", type, instance_data.uid, instance_data.position.x, instance_data.position.y + layer, instance_data.orbital_layer_delta);
    return instance_data;
}



SystemLocationData GenerateSystemLocationData(SystemBodyData *body) {

    SystemLocationData data;
    data.uid = GetUID();
    data.body_uid = body->uid;
    data.system_uid = body->system_uid;

    data.name = "location " + std::to_string(data.uid) + " on body " + std::to_string(data.body_uid);

    
    data.position = body->position;

    Vector2 location_pos = {0,0};
    float l_angle = GetRandomValue(0, 360) * DEG2RAD;
    location_pos.x = data.position.x + cosf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius) );
    location_pos.y = data.position.y + sinf(l_angle) * (body->radius - GetRandomValue(0, (int)body->radius));
    data.position = location_pos;


    

    int size = GetRandomValue(1, 10) * 10;
    data.local_data = GenerateLocationLocalData(size);

    
    data.local_data.uid = data.uid;
    data.local_data.name = data.name;
    
    data.location_plan = GenerateNewPlan(data.local_data);
    data.radius = (data.location_plan.size_x * data.location_plan.grid_size) / 2;

    printf("location data created   name: %s\n", data.name.c_str());
    return data;
}

SystemSiteData GenerateSystemSiteData(SystemLocationData *location, int uid) {

    SystemSiteData new_site;

    new_site.uid = uid;
    new_site.location_uid = location->uid;
    new_site.body_uid = location->body_uid;
    new_site.system_uid = location->system_uid;
    new_site.name = "site " +   std::to_string(new_site.uid) +  " @ location " + std::to_string(location->uid);


    new_site.radius = 16;
    
    new_site.position.x = location->position.x + location->location_plan.site_locations[uid].x - location->location_plan.px_offset.x + (location->location_plan.grid_size/2);
    new_site.position.y = location->position.y + (location->location_plan.site_locations[uid].y) - location->location_plan.px_offset.y + (location->location_plan.grid_size/2);

    new_site.local_data.uid = new_site.uid;
    new_site.local_data.name = new_site.name;

    printf("site data created    name: %s  uid: %i \n", new_site.name.c_str(), new_site.uid);
    return new_site;


}

LocationMapData GenerateLocationMapData(System *system, int location_uid) {

    SystemLocationData &sys_map_data = system->map_data.locations[location_uid];


    LocationMapData new_location;

    new_location.uid = sys_map_data.uid;
    new_location.body_uid = sys_map_data.body_uid;
    new_location.system_uid = sys_map_data.system_uid;
    new_location.radius = sys_map_data.radius * 10;
    new_location.local_data = &sys_map_data.local_data;
    new_location.name = sys_map_data.name;
    new_location.location_plan = &sys_map_data.location_plan;
    

    for(auto &[site_uid, pos] : sys_map_data.location_plan.site_locations) {

        SystemSiteData &s_site_data = system->map_data.sites[site_uid];

        LocationSiteData new_site = GenerateLocationSiteData(&s_site_data,  {(pos.x + new_location.location_plan->grid_size/2) * 10, (pos.y + new_location.location_plan->grid_size/2) * 10});
        printf("site location  name: %s  uid: %i     %0.4f %0.4f  gs: %i\n", new_site.name.c_str(), new_site.uid, pos.x, pos.y, new_location.location_plan->grid_size);

        new_location.sites[new_site.uid] = new_site;
    }
    new_location.sites;

    return new_location;
    
}

LocationSiteData GenerateLocationSiteData(SystemSiteData *s_site, Vector2 position) {
    LocationSiteData new_site;
    
    new_site.uid = s_site->uid;
    new_site.location_uid = s_site->location_uid;
    new_site.body_uid = s_site->body_uid;
    new_site.system_uid = s_site->system_uid;
    new_site.name = s_site->name;


    new_site.radius = s_site->radius * 10;
    new_site.position = position;

    new_site.local_data = &s_site->local_data;

    printf("location site data created    name: %s  uid: %i     %0.4f %0.4f\n", new_site.name.c_str(), new_site.uid, new_site.position.x, new_site.position.y);

    return new_site;
    
}