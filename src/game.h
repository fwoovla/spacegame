#pragma once

#include "resources/resources.h"
#include "data/data.hpp"
#include "render/render.hpp"
#include "viewport/viewport.hpp"
#include "scenes/scenemanager.hpp"
#include "input/input.hpp"

#define RES_X 1280.0f
#define RES_Y 720.0f

struct DebugData {
    int dummy;
    float update_time = 0.0f;
    float render_time = 0.0f;
    
};

struct SharedTransitionData {
    Vector2 return_position;
    int location_id;
};

struct GameData {
    std::string player_name = "no name";
    int tick_rate  = 10;
    bool running = false;
    bool show_debug;
    DebugData debug_data;

    SharedTransitionData transition;
    
};

extern GameData g_game_data;

class Game {
    public:
        Game();
        ~Game();
        void StartGame();
        void CleanUpGame();

        SceneManager scene_manager;
        Render render;
};

