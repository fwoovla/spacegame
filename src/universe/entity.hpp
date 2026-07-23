#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "components/components.hpp"
#include "../areas/areas.hpp"

enum ENTITY_ID {
    ENTITY_NONE = -1,
    ENTITY_PLAYER,
};


struct EntityTemplateData {
    std::string name = "no name";
    ENTITY_ID id;
    Vector2 size;
    float radius;
    bool obstructable;

    uint32_t component_flags = 0;

    HealthComponent health;
    InventoryComponent inventory;
    MovementComponent movement;
    InteractComponent interaction;

};

extern std::unordered_map<int, EntityTemplateData> g_entity_template_data;

struct EntityData {
    std::string name = "no name";
    ENTITY_ID id;
    int uid;
    Vector2 position;
    Rectangle collision_rect;
    float radius = 0.0f;
    bool obstructable = false;
    bool obstructed = false;

    uint32_t component_flags = 0;

    HealthComponent health;
    InventoryComponent inventory;
    MovementComponent movement;
    InteractComponent interaction;
};


enum BODY_TYPE {
    BODY_STAR,
    BODY_PLANET,
    BODY_MOON,
};

class SystemBodyEntity;

struct SystemBodyData {
    std::string name = "no name";
    int uid = -1;
    Vector2 position;
    Rectangle collision_rect;
    float radius = 0.0f;
    bool obstructable = false;
    bool landable = false;
    Color modulate;
    int orbiting_bodies_count = 0;
    float orbit_radius = 0.0f;
    float orbit_angle = 0.0f;
    BODY_TYPE body_type;

    std::vector<float> orbitals;
    SystemBodyEntity* parent_body = nullptr;
};



class BaseEntity  {
    public:
        virtual ~BaseEntity() = default;
        virtual void Update() = 0;   
        virtual void Draw() = 0; 
        virtual void DrawUI() = 0;


        //virtual void TakeDamage(DamagePayload _payload) = 0;

        bool should_delete = false;
        bool is_on_screen = false;
        bool y_sort = false;

        EntityData *entity_data = nullptr;
        SystemBodyData *body_data = nullptr;
};



class CreatureEntity : public BaseEntity {
    public:
    
        ~CreatureEntity() = default;

        virtual void UpdateMovement() = 0;;
        virtual void Die() {};

        Vector2 velocity = {0,0};

        bool is_stunned = false;
        RayCast raycast;
}; 



class PlayerCharacter : public CreatureEntity {
    public:
        PlayerCharacter(EntityData *_data);
        ~PlayerCharacter() override;
        void Update() override;
        void Draw() override;
        void DrawUI()override;

        void UpdateMovement() override;
        void Die() override;
    
    
    
}; 

extern PlayerCharacter * g_current_player;



struct TransitionSite {
    Vector2 position;
    std::unique_ptr<TransitionArea> transition_area;
};

class SystemBodyEntity : public BaseEntity {
    public:
    
        SystemBodyEntity(SystemBodyData *_data);
        ~SystemBodyEntity() = default;
        void Update() override;
        void Draw() override;
        void DrawUI()override;

        std::vector<std::unique_ptr<TransitionSite>> landing_sites;


}; 



int GetUID();
ENTITY_ID StrToEntityId(const std::string& s);