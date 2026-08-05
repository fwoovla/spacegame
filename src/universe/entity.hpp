#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "components/components.hpp"
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





struct SystemBodyData {
    std::string name = "no name";
    int uid = -1;
    Vector2 position;

    float radius = 0.0f;
    float orbit_radius = 0.0f;
    float orbit_angle = 0.0f;

    BODY_TYPE body_type;

    bool landable = false;
    
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
        virtual void RegisterWithManagers(SelectionManager *sm) = 0;

        bool should_delete = false;
        bool is_on_screen = false;
        bool y_sort = false;

        EntityData *entity_data = nullptr;
        SystemBodyData *body_data = nullptr;
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


 struct LandingSite {
    int uid = -1;
    std::string name = "";
    Vector2 position;
    Label label;
    TransitionArea transition_area;
};


struct BodyLocation {

    int uid = -1;
    std::string name = "";
    Vector2 position;
    Label label;
    float radius = 0.0f;
    MouseTriggerArea info_area;
    Label info_label;
    std::vector<LandingSite> landing_sites;

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
        void RegisterWithManagers(SelectionManager *sm) override;

        void OnShowInfo();
        void OnHideInfo();
        void OnShowLocationInfo();
        void OnHideLocationInfo();

        bool show_info = false;
        MouseTriggerArea info_area;
        Label info_label;

        bool show_location_info = false;

        std::vector<BodyLocation> locations;
}; 



int GetUID();
ENTITY_ID StrToEntityId(const std::string& s);