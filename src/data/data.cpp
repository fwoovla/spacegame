#include "data.hpp"
#include "../resources/json.hpp"


using json = nlohmann::json;



void LoadData() {
    LoadCreatureEntityData("assets/creature_entities.json");
    LoadObjectEntityData("assets/object_entities.json");
    LoadShipData("assets/ships.json");
    LoadShipEquipment("assets/ship_equipment.json");
    LoadCharacterControllerData("assets/characters.json");
    LoadObjectControllerData("assets/object_controllers.json");

    FuelData fuel_1;
    fuel_1.fuel_type = FUEL_CLASS_1;
    fuel_1.price = 10.0f;

    FuelData fuel_2;
    fuel_2.fuel_type = FUEL_CLASS_2;
    fuel_2.price = 20.0f;

    g_fuel_data[fuel_1.fuel_type] = fuel_1;
    g_fuel_data[fuel_2.fuel_type] = fuel_2;
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

        //new_template.ship_id = SHIP_1;
        //new_template.character_controller_id = CHARACTER_PLAYER;


        //if(controllers.contains("passive")) {
        //new_template.passive_id = StrToPassiveId(controllers["passive"]);
        //}

        g_entity_template_data[new_template.id] = new_template;
        
        printf("--ENTITY LOADED: id: %i  name: %s  \n", new_template.id, new_template.name.c_str() );   
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
        new_template.radius = e["radius"];
        new_template.max_fuel = e["max_fuel"];

        new_template.equipment_tags.resize(EQUIPMENT_COUNT);

        for(auto &eq : e["equipment"]) {


            std::string equipment_id_s = eq.begin().key();
            std::string part_id_s = eq.begin().value();

            SHIP_EQUIPMENT_ID e_id = StrToShipEquipmentId(equipment_id_s);
            SHIP_PART_ID p_id = StrToShipPartId(part_id_s);


            ShipEquipmentTag new_tag = {.equipment_id = e_id, .part_id = p_id};

            new_template.equipment_tags[e_id] = new_tag;

        }

        g_ship_template_data[new_template.id] = new_template;
        
        printf("--SHIP DATA LOADED: id: %i  name: %s  radius: %0.4f\n", new_template.id, new_template.name.c_str(), new_template.radius);   
    }

    printf("LOADED: %i SHIPS\n\n", g_ship_template_data.size());

}



void LoadShipEquipment(std::string file_path) {

    std::ifstream file(file_path);

    if(!file.is_open()) {
        printf("could not load drives\n");
        return;
    }

    printf("\n\nLOADING SHIP EQUIPMENT DATA FROM %s\n", file_path.c_str());

    json j;
    file >> j;

    

    for(auto& e : j["data"]) {

        SHIP_EQUIPMENT_ID equipment_id = StrToShipEquipmentId(e["equipment_id"].get<std::string>());

        if(equipment_id == EQUIPMENT_SYSTEM_DRIVE) {
            SystemDriveData drive = LoadSystemDriveData(e);
            g_system_drive_data[drive.part_id] = drive;
            g_ship_equipment_data[drive.part_id] = &g_system_drive_data[drive.part_id];
        }
/*         
        else if( is another SHIP_EQUIPMENT_ID) {
            load data
        }
*/

    }

    printf("LOADED: %i SHIP EQUIPMENT\n\n", g_ship_equipment_data.size());
}




SystemDriveData LoadSystemDriveData(json &e) {
    SystemDriveData drive;

    drive.equipment_id = StrToShipEquipmentId(e["equipment_id"].get<std::string>());

    drive.part_id = StrToShipPartId( e["part_id"].get<std::string>());

        drive.name = e["name"];
        drive.value = e["value"];

        auto& stats = e["stats"];

        drive.thrust = stats["thrust"];
        drive.reverse_thrust = stats["reverse_thrust"];
        drive.max_speed = stats["max_speed"];
        drive.turn_speed = stats["turn_speed"];
        drive.drag = stats["drag"];

        return drive;
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
        
        CharacterTemplateData new_template;

        new_template.id = StrToCharacterId(e["id"]);
        new_template.name = e["name"];

        new_template.speed = e["speed"];

        new_template.radius = e["radius"];

        g_character_template_data[new_template.id] = new_template;
        
        printf("--CHARACTER DATA LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i CHARACTERs\n\n", g_character_template_data.size());


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