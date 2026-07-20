#include "components.hpp"


using json = nlohmann::json;




void to_json(json& j, const HealthComponent& h) {
    j = json{
        {"current", h.health},
        {"max", h.max_health}
    };
}



void to_json(json& j, const MovementComponent& m) {
    j = json{
        {"speed", m.speed}
    };
}



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