#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "components/components.hpp"
#include "../areas/areas.hpp"
#include "../sprite/sprite.h"
//#include "location.hpp"

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
    RenderMode render_mode;

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

    RenderMode render_mode;

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





struct SystemBodyData {
    std::string name = "no name";
    int uid = -1;
    Vector2 position;

    float radius = 0.0f;
    float orbit_radius = 0.0f;
    float orbit_angle = 0.0f;

    BODY_TYPE body_type;

    bool landable = false;

    //std::vector<LandingSiteData> landing_sites;

    //std::vector<LandingSiteData> landing_sites;

    
    int orbital_layer_count = 0;
    float orbital_layer_delta;
    int orbital_body_count = 0;
    std::vector<int> orbital_bodies_uid;
    
    bool obstructable = false;
    Color modulate;
    
    
    int parent_orbital = 0;  //where it is in relation to it's parent orbitals
    int parent_body_uid = -1;
    SystemBodyData* parent = nullptr;


};



class BaseEntity  {
    public:
        virtual ~BaseEntity() = default;
        virtual void Update() = 0;   
        virtual void Draw() = 0; 
        virtual void DrawOverlay() = 0;
        virtual void DrawUI() = 0;
        virtual float GetRenderScale() = 0;


        //virtual void TakeDamage(DamagePayload _payload) = 0;

        bool should_delete = false;
        bool is_on_screen = false;
        bool y_sort = false;

        //RenderMode mode;

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
        void DrawOverlay() override;
        void DrawUI()override;

        void UpdateMovement() override;
        void Die() override;

        float GetRenderScale() override;
    
    
    
}; 

extern PlayerCharacter * g_current_player;


/* 
struct TransitionSite {
    Vector2 position;
    std::unique_ptr<TransitionArea> transition_area;
};
 */


 struct LandingSite {
    int uid = -1;
    std::string name = "";
    Vector2 position;
    Label label;
    std::unique_ptr<TransitionArea> transition_area;
};


struct BodyLocation {

    int uid = -1;
    std::string name = "";
    Vector2 position;
    Label label;
    float radius = 0.0f;
    MouseTriggerArea info_area;
    Label info_label;
    std::vector<std::unique_ptr<LandingSite>> landing_sites;

};

struct LocationMapData;

class SystemBodyEntity : public BaseEntity {
    public:
    
        SystemBodyEntity(SystemBodyData *_data);
        void GenerateLocation(LocationMapData &location);
        ~SystemBodyEntity() = default;
        void Update() override;
        void Draw() override;
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;

        void OnShowInfo();
        void OnHideInfo();
        void OnShowLocationInfo();
        void OnHideLocationInfo();

        //SystemBodyData* parent = nullptr;

        bool show_info = false;
        MouseTriggerArea info_area;
        Label info_label;

        bool show_location_info = false;


        std::vector<std::unique_ptr<BodyLocation>> locations;
        //std::vector<std::unique_ptr<LandingSite>> landing_sites;
}; 



int GetUID();
ENTITY_ID StrToEntityId(const std::string& s);