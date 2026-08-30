#include "data.hpp"
#include "../resources/json.hpp"


using json = nlohmann::json;



void LoadData() {
    LoadCreatureEntityData("assets/creature_entities.json");
    LoadObjectEntityData("assets/object_entities.json");
    LoadShipControllerData("assets/ship_controllers.json");
    LoadCharacterControllerData("assets/character_controllers.json");
    LoadObjectControllerData("assets/object_controllers.json");
}

void LoadCreatureEntityData(std::string file_path) {

    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN ENTITY DATA FILE");
        return;
    }

    printf("\n\nLOADING ENTITY DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        EntityTemplateData new_template;

        new_template.id = StrToEntityId(e["id"]);
        new_template.name = e["name"];

        if(e.contains("credits"))
            new_template.credits = e["credits"];

        if(e.contains("obstructable"))
            new_template.obstructable = e["obstructable"];

        if(e.contains("render_mode"))
            new_template.render_mode = e["render_mode"];

        auto &controllers = e["controllers"];

        if(controllers.contains("ship")) {
            new_template.ship_controller_id = StrToShipControllerId(controllers["ship"]);
        }

        if(controllers.contains("character")) {
            new_template.character_controller_id = StrToCharacterControllerId(controllers["character"]);
        }

        //if(controllers.contains("passive")) {
        //new_template.passive_id = StrToPassiveId(controllers["passive"]);
        //}

        g_entity_template_data[new_template.id] = new_template;
        
        printf("--ENTITY LOADED: id: %i  name: %s   sc: %i  cc: %i\n", new_template.id, new_template.name.c_str(), new_template.ship_controller_id, new_template.character_controller_id);   
    }

    printf("LOADED: %i ENTITIES\n\n", g_entity_template_data.size());
}


void LoadObjectEntityData(std::string file_path) {

    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN ENTITY DATA FILE");
        return;
    }

    printf("\n\nLOADING OBJECT ENTITY DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        EntityTemplateData new_template;

        new_template.id = StrToEntityId(e["id"]);
        new_template.name = e["name"];


        if(e.contains("obstructable"))
            new_template.obstructable = e["obstructable"];

        if(e.contains("render_mode"))
            new_template.render_mode = e["render_mode"];

        auto &controllers = e["controllers"];

        if(controllers.contains("object")) {
            new_template.object_entity_controller_id = StrToObjectEntityControllerId(controllers["object"]);
        }

        g_entity_template_data[new_template.id] = new_template;
        
        printf("--ENTITY LOADED: id: %i  name: %s   oc: %i\n", new_template.id, new_template.name.c_str(), new_template.object_entity_controller_id);   
    }
    
    printf("LOADED: %i OBJECT ENTITIES\n\n", g_entity_template_data.size());

}

void LoadShipControllerData(std::string file_path) {


    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN SHIPS DATA FILE");
        return;
    }

    printf("\n\nLOADING SHIPS DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        ShipControllerTemplateData new_template;

        new_template.id = StrToShipControllerId(e["id"]);
        new_template.name = e["name"];


        new_template.value = e["value"];

        new_template.radius = e["radius"];


        for(auto &m : e["flight_modes"]) {

            if(m["mode"] == 0) {
                new_template.system_drive.max_speed = m["max_speed"];
                new_template.system_drive.thrust = m["thrust"];
                new_template.system_drive.reverse_thrust = m["reverse_thrust"];
                new_template.system_drive.turn_speed = m["turn_speed"];
            }
            if(m["mode"] == 1) {
                new_template.planet_drive.max_speed = m["max_speed"];
                new_template.planet_drive.thrust = m["thrust"];
                new_template.planet_drive.reverse_thrust = m["reverse_thrust"];
                new_template.planet_drive.turn_speed = m["turn_speed"];
            }
            
        }


        g_ship_controller_template_data[new_template.id] = new_template;
        
        printf("--SHIP DATA LOADED: id: %i  name: %s  radius: %0.4f\n", new_template.id, new_template.name.c_str(), new_template.radius);   
    }

    printf("LOADED: %i SHIPS\n\n", g_ship_controller_template_data.size());

}









void LoadCharacterControllerData(std::string file_path) {


    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN CHARACTER DATA FILE");
        return;
    }

    printf("\n\nLOADING CHARACTER DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        CharacterControllerTemplateData new_template;

        new_template.id = StrToCharacterControllerId(e["id"]);
        new_template.name = e["name"];

        new_template.movement.speed = e["speed"];

        new_template.radius = e["radius"];

        g_character_controller_template_data[new_template.id] = new_template;
        
        printf("--CHARACTER DATA LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i CHARACTERs\n\n", g_character_controller_template_data.size());


}








void LoadObjectControllerData(std::string file_path) {


    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN OBJECT CONTROLLER DATA FILE");
        return;
    }

    printf("\n\nLOADING OBJECT CONTROLLER DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        ObjectEntityControllerTemplateData new_template;

        new_template.id = StrToObjectEntityControllerId(e["id"]);
        new_template.name = e["name"];

        new_template.movement.speed = e["speed"];

        new_template.radius = e["radius"];

        g_object_entity_controller_template_data[new_template.id] = new_template;
        
        printf("--OBJECT CONTROLLER DATA LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i OBJECT CONTROLLER\n\n", g_object_entity_controller_template_data.size());



}