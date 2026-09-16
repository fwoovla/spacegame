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


    SystemBodyData data;
    data.body_type = BODY_STAR;
    int uid = GetUID();
    data.uid = uid;
    data.obstructable = false;
    data.landable = false;
    data.position = map_data.star_position;
    data.parent_uid = uid;

    data.name = "star " + std::to_string(uid);
    data.radius = 8000.0f;
    
    data.orbital_body_count = GetRandomValue(1,10);
    data.orbital_layer_count = GetRandomValue(10, 20);
    data.orbital_layer_delta = map_data.radius / data.orbital_layer_count;

    data.modulate = ORANGE;

    
    printf("star  %0.5f %0.5f  delta %0.5f\n", data.position.x, data.position.y, data.orbital_layer_delta);
    return data;

}

//
//==========================BODY DATA =========================================
//
SystemBodyData GenerateSystemBodyData( BODY_TYPE type, int layer, float layer_delta, SystemBodyData *parent) {

    SystemBodyData data;
    data.body_type = type;
    int uid = GetUID();
    data.uid = uid;
    data.obstructable = false;
    data.landable = true;
    data.position = {0,0};
    data.parent_uid = parent->uid;

    if(type == BODY_PLANET) {
        data.name = "planet " + std::to_string(uid);
        GeneratePlanet(data);
    }

    else if(type == BODY_MOON) {
        data.name = "moon " + std::to_string(uid);
        GenerateMoon(data);
    }

    data.orbit_radius = layer * layer_delta;
    data.orbit_angle = DEG2RAD * GetRandomValue(0, 359);    
        
    if(parent != nullptr) {

        Vector2 pos = {0,0};
        pos.x = parent->position.x + cosf(data.orbit_angle) * data.orbit_radius;
        pos.y = parent->position.y + sinf(data.orbit_angle) * data.orbit_radius;
        data.position = pos;

        data.parent_orbital = layer;
    }

    printf("body: %i  uid: %i data created  %0.5f %0.5f  delta %0.5f\n", type, data.uid, data.position.x, data.position.y + layer, data.orbital_layer_delta);
    return data;
}



void GeneratePlanet(SystemBodyData &body_data) {
    
    body_data.body_composition = (BODY_COMPOSITION)GetRandomValue(0, BODY_COMPOSITION_COUNT - 1);
    body_data.body_environment = (BODY_ENVIRONMENT)GetRandomValue(0, BODY_ENVIRONMENT_COUNT - 1);
    

    std::vector<Color> colors = GetBodyColors(body_data);

    body_data.modulate = colors[GetRandomValue(0, colors.size() - 1)];

    body_data.radius = (float)GetRandomValue(1500, 4000);

    if(body_data.body_composition == SUPEREARTH) {
        body_data.radius *= 2;
    }

    if(body_data.body_composition != SUPEREARTH and body_data.body_composition != TERRESTRIAL) {
        body_data.landable = false;
    }

    body_data.orbital_body_count = GetRandomValue(0,5);
    body_data.orbital_layer_count = GetRandomValue(5, 20);
    body_data.orbital_layer_delta = (body_data.radius * 5) /body_data.orbital_layer_count;
    printf("body comp %s\n", CompositionTypeToStr(body_data.body_composition).c_str());
}



void GenerateMoon(SystemBodyData &body_data) {
    body_data.body_composition = (BODY_COMPOSITION)GetRandomValue(0, BODY_COMPOSITION_COUNT - 1);
    body_data.body_environment = (BODY_ENVIRONMENT)GetRandomValue(0, BODY_ENVIRONMENT_COUNT - 1);

    if(body_data.body_composition == SUPEREARTH) {
        body_data.body_composition = TERRESTRIAL;
    }

    if(body_data.body_composition != TERRESTRIAL) {
        body_data.landable = false;
    }

    std::vector<Color> colors = GetBodyColors(body_data);

    body_data.modulate = colors[GetRandomValue(0, colors.size() - 1)];

    body_data.radius = (float)GetRandomValue(500, 1200);

    body_data.orbital_body_count = 0;
    body_data.orbital_layer_count = 0;
    body_data.orbital_layer_delta = 0;

    printf("body comp %s\n", CompositionTypeToStr(body_data.body_composition).c_str());

}











//
//==========================LOCATION DATA =========================================
//
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
    
    data.location_plan = GenerateNewLocationPlan(data.local_data);
    data.radius = (data.location_plan.size_x * data.location_plan.grid_size) / 2;

    printf("location data created   name: %s\n", data.name.c_str());
    body->location_uids.push_back(data.uid);
    return data;
}






//
//==========================SITE DATA =========================================
//
SystemSiteData GenerateSystemSiteData(SystemLocationData *location, int uid) {

    SystemSiteData new_site;

    new_site.uid = uid;
    new_site.location_uid = location->uid;
    new_site.body_uid = location->body_uid;
    new_site.system_uid = location->system_uid;
    new_site.name = "site " +   std::to_string(new_site.uid) +  " @ location " + std::to_string(location->uid);

    new_site.position.x = location->position.x + location->location_plan.site_locations[uid].x - location->location_plan.px_offset.x + (location->location_plan.grid_size/2);
    new_site.position.y = location->position.y + (location->location_plan.site_locations[uid].y) - location->location_plan.px_offset.y + (location->location_plan.grid_size/2);
    
    new_site.local_data.uid = new_site.uid;
    new_site.local_data.name = new_site.name;

    new_site.local_data = GenerateSiteLocalData();  
    new_site.site_plan = GenerateNewSitePlan(new_site.local_data);
    new_site.radius = 10;

    

    printf("site data created    name: %s  uid: %i \n", new_site.name.c_str(), new_site.uid);
    return new_site;


}






std::string StarTypeToStr(STAR_TYPE star_type) {


    static const std::unordered_map<STAR_TYPE, std::string> lookup_table = {
        {STAR_TYPE::REDDWARF,                       "Red Dwarf"},   
        {STAR_TYPE::WHITEDWARF,                       "Red Dwarf"},
        {STAR_TYPE::BLUESTAR,                       "Blue Star"},
        {STAR_TYPE::GIANT,                       "Red Giant"},
        {STAR_TYPE::YELLOWSTAR,                       "Red Yellow Star"},
        {STAR_TYPE::NEUTRONSTAR,                       "Neutron Star"},
        {STAR_TYPE::BLACKHOLE,                       "Black Hole"},

    };

    if (auto it = lookup_table.find(star_type); it != lookup_table.end()) {
        return it->second;
    }
    return "star type not found";
}


std::string CompositionTypeToStr(BODY_COMPOSITION composition_type) {


    static const std::unordered_map<BODY_COMPOSITION, std::string> lookup_table = {
        {BODY_COMPOSITION::TERRESTRIAL,                       "Terrestrial"},
        {BODY_COMPOSITION::SUPEREARTH,                       "SuperEarth"},
        {BODY_COMPOSITION::NEPTUNIAN,                       "Neptunin"},
        {BODY_COMPOSITION::GASGIANT,                       "Gas Giant"},

    };

    if (auto it = lookup_table.find(composition_type); it != lookup_table.end()) {
        return it->second;
    }
    return "composition type not found";
}


std::string EnvoronmentTypeToStr(BODY_ENVIRONMENT environment_type) {


    static const std::unordered_map<BODY_ENVIRONMENT, std::string> lookup_table = {
        {BODY_ENVIRONMENT::TEMPERATE,                       "Temperate"},
        {BODY_ENVIRONMENT::DESERT,                       "Desert"},
        {BODY_ENVIRONMENT::OCEAN,                       "Ocean"},
        {BODY_ENVIRONMENT::FROZEN,                       "Frozen"},
        {BODY_ENVIRONMENT::VOLCANIC,                       "Volcanic"},
        {BODY_ENVIRONMENT::TOXIC,                       "Toxic"},
    };


    if (auto it = lookup_table.find(environment_type); it != lookup_table.end()) {
        return it->second;
    }
    return "environment type not found";
}





std::vector<Color> GetBodyColors(SystemBodyData &body_data) {
    BODY_COMPOSITION composition = body_data.body_composition;
    BODY_ENVIRONMENT environment = body_data.body_environment;

    switch (composition) {
        case TERRESTRIAL:
            return {
                PLANET_BLUE,
                PLANET_CYAN,
                PLANET_TEAL,
                PLANET_GREEN,
                PLANET_OLIVE,
                PLANET_SAND,
                PLANET_BROWN
            };

        case SUPEREARTH:
            return {
                PLANET_GREEN,
                PLANET_DARKGREEN,
                PLANET_OLIVE,
                PLANET_RED,
                PLANET_ORANGE,
                PLANET_BROWN,
                PLANET_PURPLE
            };

        case NEPTUNIAN:
            return {
                PLANET_BLUE,
                PLANET_DARKBLUE,
                PLANET_CYAN,
                PLANET_TEAL,
                PLANET_PURPLE,
                PLANET_DARKPURPLE,
                PLANET_ICE
            };

        case GASGIANT:
            return {
                PLANET_ORANGE,
                PLANET_YELLOW,
                PLANET_SAND,
            };


        default:
            return {
                PLANET_GRAY
            };
    }


}




















//
//========================== LOCATION LOCATION LOCATION =========================================
//

LocationMapData GenerateLocationMapData(System *system, int location_uid) {

    SystemLocationData &sys_map_data = system->map_data.locations[location_uid];

    SystemBodyData body_data = system->map_data.bodies[sys_map_data.body_uid];


    LocationMapData new_location;

    new_location.uid = sys_map_data.uid;
    new_location.body_uid = sys_map_data.body_uid;
    new_location.system_uid = sys_map_data.system_uid;

    new_location.radius = sys_map_data.radius * 10;

    new_location.local_data = &sys_map_data.local_data;
    new_location.name = sys_map_data.name;
    new_location.location_plan = &sys_map_data.location_plan;

    new_location.location_environment = body_data.body_environment;
    new_location.modulate = body_data.modulate;
    

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










