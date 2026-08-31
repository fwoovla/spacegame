#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../controllers/controllers.hpp"
#include "components/components.hpp"
#include "../areas/areas.hpp"
#include "../sprite/sprite.h"
//#include "../controllers/controllers.hpp"
#include "../input/selectionmanager.hpp"

enum ENTITY_ID {
    ENTITY_NONE = -1,
    ENTITY_PLAYER,
    ENTITY_ASTEROID,
};


struct EntityTemplateData {
    std::string name = "no name";
    ENTITY_ID id;
    int credits = 0;

    bool obstructable;
    RenderMode render_mode;

    SHIP_ID ship_controller_id = SHIP_NONE;
    CHARACTER_ID character_controller_id = CHARACTER_NONE;
    OBJECTENTITY_ID object_entity_controller_id = OBJECTENTITY_NONE;
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

    RenderMode render_mode = RENDER_WORLD;

    SHIP_ID ship_controller_id = SHIP_NONE;
    CHARACTER_ID character_controller_id = CHARACTER_NONE;
    OBJECTENTITY_ID object_entity_controller_id = OBJECTENTITY_NONE;
};




class BaseEntity  {
    public:

        enum MOVEMENT_TYPE {
            MOVEMENT_CHARACTER,
            MOVEMENT_SHIP,
            MOVEMENT_OBJECTENTITY
        };

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

        SelectionManager *selection_manager = nullptr;

        EntityData *entity_data = nullptr;

        
};





class ObjectEntity : public BaseEntity { //

    public:
    ObjectEntity(EntityData *_data);
    ~ObjectEntity() override;
    void Update() override;
    void Draw() override;
    void DrawOverlay() override;
    void DrawUI()override;

    float GetRenderScale() override;
    void RegisterWithManagers(SelectionManager *sm) override;


    ObjectEntityControllerData object_controller_data;
    std::unique_ptr<ObjectEntityController> object_controller;

};




class CreatureEntity : public BaseEntity {
    public:
            
        ~CreatureEntity() = default;

        virtual void UpdateMovement() = 0;
        virtual void EnterShip() = 0;
        virtual void ExitShip() = 0;
        virtual void Die() = 0;

        MOVEMENT_TYPE movement_type = MOVEMENT_SHIP;

        ShipControllerData ship_controller_data;
        std::unique_ptr<ShipController> ship_controller;

        CharacterControllerData character_controller_data;
        std::unique_ptr<CharacterController> character_controller;

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
        void Die() override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

}; 

extern PlayerCharacter * g_current_player;



ENTITY_ID StrToEntityId(const std::string& s);

OBJECTENTITY_ID StrToObjectEntityId(const std::string& s);