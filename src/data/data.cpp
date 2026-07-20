#include "data.hpp"
#include "../resources/json.hpp"
#include "../universe/entity.hpp"

using json = nlohmann::json;



void LoadData() {
    LoadEntityData("assets/entities.json");
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

        new_template.size.x = (float)e["size_x"];
        new_template.size.y = (float)e["size_y"];


        if (e.contains("components")) {

            auto &c = e["components"];

            if(c.contains("health")) {
                //new_template.has_health = true;
                new_template.component_flags |= HAS_HEALTH;
                new_template.health.max_health = c["health"]["max"];
            }
            if(c.contains("movement")) {
                //new_template.has_movement = true;
                new_template.component_flags |= HAS_MOVEMENT;
                new_template.movement.speed = c["movement"]["speed"];
                printf("speed = %0.3f\n", new_template.movement.speed);
            }
            if(c.contains("inventory")) {
                //new_template.has_inventory = true;
                new_template.component_flags |= HAS_INVENTORY;
            }
            if(c.contains("interaction")) {
                //new_template.has_interaction = true;
                new_template.component_flags |= HAS_INTERACTION;
                new_template.interaction.range = c["interaction"]["range"];
                new_template.interaction.can_interact = c["interaction"]["can_interact"];
                new_template.interaction.highlightable = c["interaction"]["highlightable"];
                new_template.interaction.priority = c["interaction"]["priority"];
                new_template.interaction.type = c["interaction"]["type"];
            }
        }

        g_entity_template_data[new_template.id] = new_template;
        
        printf("--ENTITY LOADED: id: %i  name: %s\n", new_template.id, new_template.name.c_str());   
    }
    printf("LOADED: %i ENTITIES\n\n", g_entity_template_data.size());
}
