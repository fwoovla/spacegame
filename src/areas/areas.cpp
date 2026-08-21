#include "areas.hpp"
//#include "../game.h"

/////TRANSITION AREA
TransitionArea::~TransitionArea() {
    //TraceLog(LOG_INFO, "TransitionArea AREA DESTROYED");
}

void TransitionArea::Update() {
    hovered = false;
    

}



bool TransitionArea::Contains(Vector2 mouse_pos)
{
    switch(shape)
    {
        case CIRCLE:
            return CheckCollisionPointCircle(mouse_pos, position, radius);

        case RECT:
            return CheckCollisionPointRec(mouse_pos, {
                position.x,
                position.y,
                size.x,
                size.y
            });
    }

    return false;
}




//-----------------MOUSE TRIGGER AREA------------------------------
//used to detect mouse-over. nothing else





MouseTriggerArea::~MouseTriggerArea() {
    //TraceLog(LOG_INFO, "MouseTriggerArea AREA DESTROYED");
}

void MouseTriggerArea::Update() {

}




bool MouseTriggerArea::Contains(Vector2 mouse_pos)
{
    switch(shape)
    {
        case CIRCLE:
            return CheckCollisionPointCircle(mouse_pos, position, radius);

        case RECT:
            return CheckCollisionPointRec(mouse_pos, {
                position.x,
                position.y,
                size.x,
                size.y
            });
    }

    return false;
}












