#include "../controllers.hpp"
#include "../../game.h"

ObjectEntityController::ObjectEntityController(ObjectEntityControllerData *_data) {

    object_controller_data = _data;
}

ObjectEntityController::~ObjectEntityController() {

}

void ObjectEntityController::Update(Vector2 &position) {
/* 
    Vector2 input_dir = {0,0};

    if(g_input.key_up) {input_dir.y = -1;}
    if(g_input.key_down) {input_dir.y = 1;}
    if(g_input.key_left) {input_dir.x = -1;}
    if(g_input.key_right) {input_dir.x = 1;}     */

/*     Vector2 next_position = position;
    
    if(object_controller_data->movement.velocity.x != 0 or object_controller_data->movement.velocity.y != 0 ) {
        next_position = Vector2Add(next_position, object_controller_data->movement.velocity * GetFrameTime());
    }

    position = next_position;

    float speed =  object_controller_data->movement.speed; */
/* 
    if( isnan(object_data->movement.velocity.x) || isnan(object_data->movement.velocity.y)) {object_data->movement.velocity = {0,0};}
        
    object_data->movement.velocity = Vector2Lerp(object_data->movement.velocity, input_dir * speed, .15);
    object_data->movement.velocity.y = {0.0}; */
    
/*     printf("character movement\n");
    printf("input %.1f %.1f  velocity %.1f %.1f  position %.1f %.1f speed %0.4f\n",
    input_dir.x,
    input_dir.y,
    character_data->movement.velocity.x,
    character_data->movement.velocity.y,
    position.x,
    position.y,
    speed); */
}

void ObjectEntityController::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);    
    DrawCircleV(screen, object_controller_data->radius * scale, RAYWHITE);
    printf("OBJECT DRAW  %0.3f  %0.3f  %0.3f\n", screen.x, screen.y, object_controller_data->radius);

}

