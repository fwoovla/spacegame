#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
//#include "components/components.hpp"
#include "../areas/areas.hpp"
#include "../sprite/sprite.h"
#include "../ships/ships.hpp"
#include "../characters/characters.hpp"
#include "../input/selectionmanager.hpp"

enum ENTITY_ID {
    ENTITY_NONE = -1,
    ENTITY_PLAYER,
};


struct EntityTemplateData {
    std::string name = "no name";
    ENTITY_ID id;
    int credits = 0;

    bool obstructable;
    RenderMode render_mode;
};

extern std::unordered_map<int, EntityTemplateData> g_entity_template_data;

struct EntityData {
    int uid;
    std::string name = "no name";
    ENTITY_ID id;
    int credits = 0;

    Vector2 position;
    bool obstructable = false;
    bool obstructed = false;

    RenderMode render_mode;
};


enum BODY_TYPE {
    BODY_STAR,
    BODY_PLANET,
    BODY_MOON,
};




//struct SystemBodyMapData;
struct SystemBodyLocalData;

struct SystemBodyData {
    SystemBodyLocalData local_data;  //economy population etc...

    int uid = -1;
    int system_uid;
    std::string name = "no body name";

    //SystemBodyMapData map_data;

    Vector2 position;

    float radius = 0.0f;
    float orbit_radius = 0.0f;
    float orbit_angle = 0.0f;

    BODY_TYPE body_type;

    bool landable = false;
    
    int orbital_layer_count = 0;
    float orbital_layer_delta;
    int orbital_body_count = 0;
    std::vector<int> orbital_body_uids;
    std::vector<int> location_uids;
    
    bool obstructable = false;
    Color modulate;
    
    int parent_orbital = 0;  //where it is in relation to it's parent orbitals
    int parent_uid = -1;

    
};



//struct SystemBodyMapData;
struct SystemLocationLocalData;

struct SystemLocationData {
    SystemLocationLocalData local_data;  //economy population etc...

    int uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    std::string name = "no location name";
    Vector2 position;

    float system_radius = 0.0f;
    float location_radius = 0.0f;

};


struct SystemSiteLocalData;

struct SystemSiteData {
    SystemSiteLocalData local_data;
    int uid = -1;
    int location_uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    std::string name = "no site name";
    Vector2 position;

    float system_radius = 0.0f;
    float location_radius = 0.0f;
};





class BaseEntity  {
    public:
        virtual ~BaseEntity() = default;
        virtual void Update() = 0;   
        virtual void Draw() = 0; 
        virtual void DrawOverlay() = 0;
        virtual void DrawUI() = 0;
        virtual float GetRenderScale() = 0;
        virtual void RegisterWithManagers(SelectionManager *sm) = 0;

        bool should_delete = false;
        bool is_on_screen = false;
        bool y_sort = false;

        MouseTriggerArea info_area;
        Label info_label;

        //EntityData *entity_data = nullptr;
        //SystemBodyInstanceData *body_data = nullptr;
        SelectionManager *selection_manager = nullptr;
};


class CreatureEntity : public BaseEntity {
    public:
        enum MOVEMENT_TYPE {
            MOVEMENT_CHARACTER,
            MOVEMENT_SHIP,
        };
    
        ~CreatureEntity() = default;

        virtual void UpdateMovement() = 0;
        //virtual void UpdateShipMovement() = 0;
        //virtual void UpdateCharacterMovement() = 0;
        virtual void EnterShip() = 0;
        virtual void ExitShip() = 0;
        virtual void Die() = 0;

        EntityData *entity_data = nullptr;

        MOVEMENT_TYPE movement_type = MOVEMENT_SHIP;

        std::unique_ptr<Ship> ship;
        std::unique_ptr<Character> character;

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
        void EnterShip() override;
        void ExitShip() override;
        //void UpdateShipMovement() override;
        //void UpdateCharacterMovement() override;
        void Die() override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

}; 

extern PlayerCharacter * g_current_player;



class SystemBodyEntity : public BaseEntity {
    public:
        SystemBodyEntity(SystemBodyData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemBodyEntity() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemBodyData *body_data = nullptr;

        SystemBodyEntity *parent;
}; 


class SystemLocationEntity : public BaseEntity {
    public:
        SystemLocationEntity(SystemLocationData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemLocationEntity() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemLocationData *location_data = nullptr;

        SystemBodyEntity *parent;
}; 

class SystemSiteEntity : public BaseEntity {
    public:
        SystemSiteEntity(SystemSiteData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemSiteEntity() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemSiteData *site_data = nullptr;

        SystemBodyEntity *parent;
}; 

int GetUID();
ENTITY_ID StrToEntityId(const std::string& s);