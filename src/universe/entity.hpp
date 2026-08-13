#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../ships/ships.hpp"
//#include "components/components.hpp"
#include "../areas/areas.hpp"
#include "../sprite/sprite.h"
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
        void Die() override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

}; 

extern PlayerCharacter * g_current_player;



ENTITY_ID StrToEntityId(const std::string& s);