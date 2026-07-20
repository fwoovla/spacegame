#pragma once

#include <raylib.h>
#include <string>
#include "../scenes/scenemanager.hpp"

struct WindowSettings {
    Vector2 size = {100, 100};
    Vector2 resolution = {100, 100};
    Vector2 center;
    std::string title = "no title";
    float scale = 1.0f;
    float inv_scale = 0.0f;

};

//extern WindowSettings g_window_settings;

class Render {
    
    public:

    RenderTexture2D scene_render_texture = {};
    RenderTexture2D ui_render_texture = {};
    float scale = 0.0f;
    float inv_scale = 0.0f;
    int offsetX = 0;
    int offsetY = 0;

    void Init(Vector2 resolution);

    void DrawWindow(SceneManager &_sm);

    WindowSettings current_window;
};

