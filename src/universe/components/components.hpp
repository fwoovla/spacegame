#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "../../resources/json.hpp"

using json = nlohmann::json;



enum ComponentFlags
{
    COMPONENT_NONE      = 0,
    COMPONENT_HEALTH    = 1 << 0,
    COMPONENT_INVENTORY = 1 << 1,
    COMPONENT_INTERACT = 1 << 2
};


/* struct MovementComponent {
    float speed = 0.0f;
}; */

struct HealthComponent
{
    float health = 0.0f;
    float max_health = 0.0f;
};


struct InventoryComponent
{
    std::vector<int> inventory;
};



struct InteractComponent
{
    float range = 16.0f;
    bool can_interact = true;
    bool highlightable = true;
    int priority = 0; // higher = wins selection

    enum Type
    {
        NONE,
        HARVEST,
        TALK,
        OPEN,
        ATTACK
    } type = NONE;
};





void to_json(json& j, const HealthComponent& h);
//void to_json(json& j, const MovementComponent& m);
void to_json(json& j, const InventoryComponent& i);
void to_json(json& j, const InteractComponent& i);

void from_json(const json& j, HealthComponent& h);
//void from_json(const json& j, MovementComponent& m);
void from_json(const json& j, InventoryComponent& i);
void from_json(const json& j, InteractComponent& i);