#pragma once 
#include <raylib.h>

extern Camera2D g_camera;

struct Viewport {
    int tile_size = 0;
    int x_offset_i;
    int y_offset_i;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    float x_offset_f;
    float y_offset_f;
    float inv_tile_size;
    float world2screen;
    float screen2world;
    Vector2 center_i;
    Vector2 center_f;
    Vector2 screen_size;
    Vector2 resolution;
    float scale;
    float inv_scale;
    int map_width;// = g_game_data.world_settings.world_size; // Replace with actual map width
    int map_height;// = g_game_data.world_settings.world_size; // Replace with actual map height
    int chunk_x;
    int chunk_y;
};

extern Viewport g_viewport;

void CalculateViewport(Vector2 resolution, float scale);

void HandleCamera();



