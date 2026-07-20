#pragma once
#include <raylib.h>
#include <vector>
#include "../../resources/json.hpp"

using json = nlohmann::json;



enum ComponentFlags
{
    HAS_HEALTH    = 1 << 0,
    HAS_MOVEMENT  = 1 << 1,
    HAS_INVENTORY = 1 << 2,
    HAS_INTERACTION = 1 << 3
};


struct MovementComponent
{
    float speed;
};

struct HealthComponent
{
    int health;
    int max_health;
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

void to_json(json& j, const MovementComponent& m);

void to_json(json& j, const InventoryComponent& i);

void to_json(json& j, const InteractComponent& i);