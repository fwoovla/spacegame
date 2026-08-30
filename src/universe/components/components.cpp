#include "components.hpp"


using json = nlohmann::json;




void to_json(json& j, const HealthComponent& h) {
    j = json{
        {"current", h.health},
        {"max", h.max_health}
    };
}


/* 
void to_json(json& j, const MovementComponent& m) {
    j = json{
        {"speed", m.speed}
    };
}
 */


void to_json(json& j, const InventoryComponent& i) {
    j = json{
        {"inventory", i.inventory}        
    };
}

void to_json(json& j, const InteractComponent& i) {
    j = json{
        {"range", i.range},
        {"can_interact", i.can_interact},
        {"highlightable", i.highlightable},
        {"priority", i.priority},
        {"type", i.type},
    };

}



void from_json(const json& j, HealthComponent& h)
{
    j.at("current").get_to(h.health);
    j.at("max").get_to(h.max_health);
}

/* void from_json(const json& j, MovementComponent& m)
{
    j.at("speed").get_to(m.speed);
}
 */

void from_json(const json& j, InventoryComponent& i)
{
    j.at("inventory").get_to(i.inventory);
}

void from_json(const json& j, InteractComponent& i)
{
    j.at("range").get_to(i.range);
    j.at("can_interact").get_to(i.can_interact);
    j.at("highlightable").get_to(i.highlightable);
    j.at("priority").get_to(i.priority);
    j.at("type").get_to(i.type);
}
