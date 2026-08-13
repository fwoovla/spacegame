#include "data.hpp"
#include "../resources/json.hpp"


using json = nlohmann::json;



void LoadData() {
    LoadEntityData("assets/entities.json");
    LoadShipData("assets/ships.json");
    LoadCharacterData("assets/characters.json");
}

void LoadEntityData(std::string file_path) {

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

        new_template.obstructable = e["obstructable"];
        
        new_template.render_mode = e["render_mode"];


        g_entity_template_data[new_template.id] = new_template;
        
        printf("--ENTITY LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i ENTITIES\n\n", g_entity_template_data.size());
}


void LoadShipData(std::string file_path) {


    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN SHIPS DATA FILE");
        return;
    }

    printf("\n\nLOADING SHIPS DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        ShipTemplateData new_template;

        new_template.id = StrToShipId(e["id"]);
        new_template.name = e["name"];


        new_template.value = e["value"];


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


        g_ship_template_data[new_template.id] = new_template;
        
        printf("--SHIP DATA LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }

    printf("LOADED: %i SHIPS\n\n", g_ship_template_data.size());



}





void LoadCharacterData(std::string file_path) {


    std::ifstream cfile(file_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN CHARACTER DATA FILE");
        return;
    }

    printf("\n\nLOADING CHARACTER DATA FROM %s\n", file_path.c_str());

    json j;
    cfile>>j;

    for(auto &e : j["data"]) {
        
        CharacterTemplateData new_template;

        new_template.id = StrToCharacterId(e["id"]);
        new_template.name = e["name"];

        new_template.movement.speed = e["speed"];
        

        g_character_template_data[new_template.id] = new_template;
        
        printf("--CHARACTER DATA LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i CHARACTERS\n\n", g_character_template_data.size());


}