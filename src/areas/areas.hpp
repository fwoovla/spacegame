#pragma once
#include "../ui/label.hpp"
#include "../utils/utils.hpp"


enum TRANSITION_TYPE {

    LAUNCHING,
    LANDING,
};


class BaseArea {
    public:
        virtual ~BaseArea() = default;
        virtual void Update() = 0;
        //virtual void Draw() = 0;
        //virtual void Activate() = 0;

        Vector2 position;
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
        //void Draw() override;
        //void Activate() override;

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

        enum SHAPE {
            CIRCLE,
            RECT,
        };

        MouseTriggerArea(){};
        ~MouseTriggerArea() override;
        void Update() override;
        //void Draw() override;
        //void Activate() override;
        int location_id = -1;

        bool mouse_hovering = false;

        SHAPE shape;

        Signal mouse_entered;
        Signal mouse_exited;

};