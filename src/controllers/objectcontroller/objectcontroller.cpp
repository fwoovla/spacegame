#include "../controllers.hpp"
#include "../../game.h"

ObjectEntityController::ObjectEntityController(ObjectEntityControllerData *_data) {

    object_controller_data = _data;
    object_controller_data->movement.rotation = GetRandomValue(0, 360) * DEG2RAD;
    Vector2 vel = {object_controller_data->movement.speed, 0};
    object_controller_data->movement.velocity = Vector2Rotate(vel, object_controller_data->movement.rotation);
}

ObjectEntityController::~ObjectEntityController() {

}

void ObjectEntityController::Update(Vector2 &position) {

    Vector2 previous_position = position;
    float dt = GetFrameTime();

    Vector2 &velocity = object_controller_data->movement.velocity;

    position = Vector2Add(position, velocity * dt);

}   

void ObjectEntityController::Draw(Vector2 &position, float scale) {

    //Vector2 screen = GetWorldToScreen2D(position, g_camera);    
    //DrawCircleV(screen, object_controller_data->radius, RAYWHITE);
    //printf("OBJECT DRAW  %0.3f  %0.3f  %0.3f\n", screen.x, screen.y, object_controller_data->radius);

    DrawCircleV(position, object_controller_data->radius, RAYWHITE);
    //printf("OBJECT DRAW  %0.3f  %0.3f  %0.3f\n", position.x, position.y, object_controller_data->radius);

}

