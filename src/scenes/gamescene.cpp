#include "scenes.hpp"
#include "../game.h"
#include "../universe/entity.hpp"


GameScene::GameScene() {

    scene_id = SCENE_GAME;
    return_scene = SCENE_NONE;


    universe_manager.CreateUniverse(g_game_data.player_name);

    //universe_manager.current_system->landing_requested.Connect( [&]() { OnLandingRequested();});


    
    //EntityData player_data = GenerateEntityInstance(g_entity_template_data[ENTITY_PLAYER], 0, {500, 500});
    //universe_data.system_data.entity_data[player_data.uid] = player_data;
    

    g_viewport.map_width =  universe_manager.current_system->system_data.radius * 2;
    g_viewport.map_height =  universe_manager.current_system->system_data.radius * 2;

    g_camera = {0};
    g_camera.target = g_current_player->entity_data->position;
    g_camera.offset = {g_viewport.resolution.x * 0.5f, g_viewport.resolution.y * 0.5f};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 0.5f;
    


    universe_manager.exit_ship.Connect([&]() { OnExitShip();});
    universe_manager.enter_ship.Connect([&]() { OnEnterShip();});

    //ui = std::make_unique<GameUiLayer>();
    

    world_ticker.Start(1.0f / g_game_data.tick_rate, false);
    world_ticker.timer_timeout.Connect( [&](){OnWorldTick();} );
    
    
    //HideCursor();
}


SCENE_ID GameScene::Update() {
    //return SCENE_NONE;
    return_scene = SCENE_NONE;

    world_ticker.Update();

    HandleCamera();
 
 /*    if(g_input.keys_pressed[0] == KEY_ENTER) {
        g_current_player->entity_data->position.x = (float)GetRandomValue(100, 20000);
        g_current_player->entity_data->position.y = (float)GetRandomValue(100, 20000);
    }
 */
    
    if(g_current_player != nullptr) {
        universe_manager.Update();
    }
 
    ui.Update();

    return return_scene;
}


void GameScene::DrawScene() {
   
    if(g_game_data.show_debug) {
        DrawCircleV(g_input.screen_mouse_position, 6, YELLOW);
    }

    BeginMode2D(g_camera);

    universe_manager.DrawWorld();
    
    if(g_game_data.show_debug) {
        universe_manager.DrawDebug();        

        DrawRectangleLines(0, 0, universe_manager.current_system.get()->system_data.radius * 2, universe_manager.current_system.get()->system_data.radius * 2, WHITE);
        //DrawCircleV(g_input.world_mouse_position, 4, GREEN);    
    }

    EndMode2D();
    
    universe_manager.DrawOverlay();

}


void GameScene::DrawUI() {

    universe_manager.DrawUI();
    ui.Draw();
}


GameScene::~GameScene() {
    //delete ui_layer;
    //world_manager.SaveWorld();
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GameScene");
}

void GameScene::OnWorldTick() {
    //printf("tick %i\n", g_game_data.tick_count);
    //g_game_data.tick_count++;
}




void GameScene::OnEnterShip() {
    universe_manager.hud.SetTarget(g_current_player, universe_manager.current_system.get(), &universe_manager.selection_manager);
}

void GameScene::OnExitShip() {
    universe_manager.hud.ClearTarget();
}





