#include "game.h"



GameData g_game_data;
Font g_font;
PlayerInput g_input;
Camera2D g_camera;
Viewport g_viewport;
PlayerCharacter *g_current_player;
std::unordered_map<int, EntityTemplateData> g_entity_template_data;

Game::Game() {
    printf("game created\n");

}

Game::~Game() {
    printf("game destroyed\n");
}

void Game::StartGame() {
    printf("game started\n");

    LoadResources();
    LoadData();
    g_viewport.resolution = {RES_X, RES_Y};
    float scaleX = (float)GetScreenWidth() / g_viewport.resolution.x;
    float scaleY = (float)GetScreenHeight() / g_viewport.resolution.y;
    
    g_viewport.scale = (scaleX < scaleY) ? scaleX : scaleY;
    g_viewport.inv_scale = 1/g_viewport.scale;


    scene_manager.Init();
    render.Init( {RES_X, RES_Y} );

    printf("res = %0.5f, %0.5f --------------------------------------------------------------\n", g_viewport.resolution.x, g_viewport.resolution.y);
    g_game_data.running = true;

    while(!WindowShouldClose() and g_game_data.running == true) {
        
        scene_manager.UpdateScene();

        render.DrawWindow(scene_manager);
    }
    g_game_data.running = false;


}

void Game::CleanUpGame() {
    printf("game cleanup\n");
}