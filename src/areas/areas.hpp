#pragma once
#include <raylib.h>
#include "../ui/label.hpp"
#include "../utils/utils.hpp"



enum AREA_PRIORITY
{
    PRIORITY_BODY = 0,
    PRIORITY_LOCATION = 10,
    PRIORITY_LANDING_SITE = 20,
    PRIORITY_NPC = 30
};


enum TRANSITION_TYPE {

    LAUNCHING,
    LANDING,
};


class BaseArea {
    public:

    enum SHAPE {
            CIRCLE,
            RECT,
        };
        virtual ~BaseArea() = default;
        virtual void Update() = 0;
        virtual bool Contains(Vector2 mouse_pos) = 0;

        AREA_PRIORITY priority = PRIORITY_BODY;

        SHAPE shape;
        RAYLIB_H::Vector2 position;
        Vector2 size;
        float radius = 0.0f;
        bool collided = false;
        bool hovered = false;
        bool in_range = false;

        

};


class TransitionArea : public BaseArea {

    public:
        TransitionArea(){};
        ~TransitionArea() override;
        void Update() override;
        bool Contains(Vector2 mouse_pos) override;

        TRANSITION_TYPE type;

        int location_uid = -1;
        int body_id = -1;
        int system_id = -1;

        float time_pressed;

        Signal area_entered;
        Signal area_activated;

};


 class MouseTriggerArea : public BaseArea {

    public:

        MouseTriggerArea(){};
        ~MouseTriggerArea() override;
        void Update() override;
        bool Contains(Vector2 mouse_pos) override;

        int location_id = -1;

        bool mouse_hovering = false;

        Signal mouse_entered;
        Signal mouse_exited;

};