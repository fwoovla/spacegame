#include <raylib.h>
#include <string>
#include "scenemanager.hpp"
#include "../input/input.hpp"
#include "../game.h"


std::unique_ptr<BaseScene> g_current_scene;

void SceneManager::Init() {
    g_current_scene = std::make_unique<TitleScene>();



    //is_transitioning = false;
    //can_delete_transition = false;

}


SceneManager::~SceneManager() {

    TraceLog(LOG_INFO, "cleaning up scene manager");
}



void SceneManager::UpdateScene() {
    SCENE_ID new_scene_id = SCENE_NONE;

    GetInputFromPlayer();


    double update_now = GetTime();
    new_scene_id = g_current_scene->Update();
    g_game_data.debug_data.update_time = (GetTime() - update_now) * 1000;


    if(new_scene_id != SCENE_NONE) {
        ChangeSceneTo(new_scene_id);
    }

    //g_game_data.frame_count++;


    if(g_input.keys_pressed[0] == KEY_TAB) {
        g_game_data.show_debug = !g_game_data.show_debug;
    }

    if(g_game_data.show_debug) {
        //debug_ui.Update();
    } 
}

void SceneManager::DrawScene() {
    //printf("drawing scene --sm\n");
    double now = GetTime();
    g_current_scene->DrawScene();
    g_game_data.debug_data.render_time = (GetTime() - now) * 1000;
    

}

void SceneManager::DrawUI() {

    g_current_scene->DrawUI();

/*     if(g_game_data.paused == true) {
        pause_menu->Draw();
    }
    DrawSprite(g_cursor.sprite);

    if(is_transitioning and fade_transition != nullptr) {
        fade_transition->Draw();
    }
    */
    if(g_game_data.show_debug) {
        //debug_ui.Draw();
    } 
}


void SceneManager::ChangeSceneTo(SCENE_ID new_scene_id) {
    
    
    g_current_scene.reset();
    //g_game_data.current_map_index = g_game_data.next_map_index;

    switch (new_scene_id) {
        case SCENE_SPLASH:
            g_current_scene = std::make_unique<SplashScene>();
            break;

        case SCENE_TITLE:
            g_current_scene = std::make_unique<TitleScene>();
            break;

        case SCENE_GAME:
            g_current_scene = std::make_unique<GameScene>();
            break;


        default:
            g_current_scene = std::make_unique<SplashScene>();
            break;
    }
    //g_game_data.current_scene_id = new_scene_id; */
}

