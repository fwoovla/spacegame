#include "viewport.hpp"
#include "../game.h"
#include "../input/input.hpp"



#define MAX_ZOOM 4.4f
#define MIN_ZOOM 0.005f
#define ZOOM_STEP 0.05f


float max_dist_sqr = 150*150;
float min_dist_sqr = 50*50;

void CalculateViewport(Vector2 resolution, float scale) {

    g_viewport.tile_size = 32;
    g_viewport.resolution = resolution;

    g_viewport.inv_tile_size = 1.0f/(float)g_viewport.tile_size;

    g_viewport.x_offset_i = (resolution.x * 0.5f) / g_camera.zoom * g_viewport.inv_tile_size;
    g_viewport.y_offset_i = (resolution.y * 0.5f) / g_camera.zoom * g_viewport.inv_tile_size;
    g_viewport.x_offset_f = (resolution.x * 0.5f) / g_camera.zoom;
    g_viewport.y_offset_f = (resolution.y * 0.5f) / g_camera.zoom;

    g_viewport.center_i = Vector2Add(g_camera.target, {g_viewport.x_offset_f, g_viewport.y_offset_f} ) * g_viewport.inv_tile_size;
    g_viewport.center_f = Vector2Add(g_camera.target, {g_viewport.x_offset_f, g_viewport.y_offset_f});

    
    g_viewport.x_min = g_viewport.center_i.x - (g_viewport.x_offset_i + 1);
    g_viewport.x_max = g_viewport.center_i.x + (g_viewport.x_offset_i + 2);
      
    g_viewport.y_min = g_viewport.center_i.y - (g_viewport.y_offset_i + 1);        
    g_viewport.y_max = g_viewport.center_i.y + (g_viewport.y_offset_i + 2);

    g_viewport.world2screen = g_camera.zoom * scale;
    g_viewport.screen2world = 1/g_viewport.world2screen;


}



void HandleCamera() {

    g_camera.zoom += g_input.mouse_wheel * ZOOM_STEP;

    if (g_camera.zoom < MIN_ZOOM){g_camera.zoom = MIN_ZOOM;}
    if (g_camera.zoom > MAX_ZOOM){g_camera.zoom = MAX_ZOOM;}

    CalculateViewport(g_viewport.resolution, g_viewport.scale);

    float half_w = g_viewport.x_offset_f;
    float half_h = g_viewport.y_offset_f;

        
    Vector2 new_camera_target = g_current_player->entity_data->position;
    Vector2 mouse_offset = Vector2Subtract(g_input.world_mouse_position, new_camera_target);


    mouse_offset.x = mouse_offset.x * 0.5f;
    mouse_offset.y = mouse_offset.y * 0.5f;

    float dist = Vector2DistanceSqr(g_input.world_mouse_position, new_camera_target);

    if(dist < min_dist_sqr) {
        mouse_offset.x = 0.0f;
        mouse_offset.y = 0.0f;
    }

    if(mouse_offset.x < -115) {mouse_offset.x = -115;}
    else if(mouse_offset.x > 115) {mouse_offset.x = 115;}

    if(mouse_offset.y < -70) {mouse_offset.y = -70;}

    else if(mouse_offset.y >= 70) {mouse_offset.y = 70;}
    
    new_camera_target = Vector2Add(new_camera_target, mouse_offset);

    new_camera_target.x = std::clamp(new_camera_target.x, 0.0f, (float)g_viewport.map_width);
    new_camera_target.y = std::clamp(new_camera_target.y, 0.0f, (float)g_viewport.map_height);
    
    g_camera.target.x = Lerp(g_camera.target.x, new_camera_target.x, 0.04f);
    g_camera.target.y = Lerp(g_camera.target.y, new_camera_target.y, 0.04f);

/*     printf("camera target %0.5f %0.5f   mouse offset %0.5f %0.5f\n", g_camera.target.x, g_camera.target.y, mouse_offset.x, mouse_offset.y);
    printf("player ptr %p data %p pos %f %f\n",
    g_current_player,
    g_current_player->entity_data,
    g_current_player->entity_data->position.x,
    g_current_player->entity_data->position.y); */
 
}


