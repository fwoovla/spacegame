#include "viewport.hpp"
#include "../game.h"
#include "../input/input.hpp"



#define MAX_ZOOM 20.0f
#define MIN_ZOOM 0.00025f
#define ZOOM_STEP 0.05f

#define SYSTEM_ZOOM 0.005f
#define BODY_ZOOM 8.0f
#define LOCATION_ZOOM 10.5f
#define SITE_ZOOM 18.0f


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
    float zoom_factor = g_camera.zoom/1;

    float wheel_zoom = g_input.mouse_wheel * (ZOOM_STEP) * zoom_factor;
    g_camera.zoom += wheel_zoom;
    if(wheel_zoom != 0.0f) {
        g_game_data.do_camera_transition = false;
    }

    if(g_game_data.do_camera_transition) {
        float target_zoom = SYSTEM_ZOOM;
        float step = 0.0f;

        if(g_game_data.camera_state == CAMERA_BODY) {
            target_zoom = BODY_ZOOM;
            step = 0.005f;
        }
        else if(g_game_data.camera_state == CAMERA_SYSTEM) {
            target_zoom = SYSTEM_ZOOM;
            step = 0.05f;
        }
        if(g_game_data.camera_state == CAMERA_LOCATION) {
            target_zoom = LOCATION_ZOOM;
            step = 0.005f;
        }
        else if(g_game_data.camera_state == CAMERA_SITE) {
            target_zoom = SITE_ZOOM;
            step = 0.05f;
        }
        g_camera.zoom = lerp(g_camera.zoom, target_zoom, step);
        if( abs(g_camera.zoom  - target_zoom) < (target_zoom * 0.01f)) { 
            g_game_data.do_camera_transition = false;
        }

    }

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

    float lerp_factor = 0.1f;
    
    g_camera.target.x = Lerp(g_camera.target.x, new_camera_target.x, lerp_factor);
    g_camera.target.y = Lerp(g_camera.target.y, new_camera_target.y, lerp_factor);
 
}




