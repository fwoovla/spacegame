#include "scenes.hpp"
#include "../game.h"


TitleScene::TitleScene() {

    scene_id = SCENE_TITLE;
    return_scene = SCENE_NONE;

    ui = std::make_unique<TitleUiLayer>();

    ui.get()->start_button.button_pressed.Connect([&](){OnStartButtonPressed();});
    ui.get()->exit_button.button_pressed.Connect([&](){OnExitButtonPressed();});
    ui.get()->settings_button.button_pressed.Connect([&](){OnSettingsButtonPressed();});

}


SCENE_ID TitleScene::Update() {

    return_scene = SCENE_NONE;
    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = SCENE_GAME;
    }

    ui->Update();

    return return_scene;
}


void TitleScene::DrawScene() {

}


void TitleScene::DrawUI() {
    ui->Draw();
}


TitleScene::~TitleScene() {
    //delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TITLE");
}


void TitleScene::OnStartButtonPressed() {
    return_scene = SCENE_GAME;
}

void TitleScene::OnExitButtonPressed() {
    return_scene = SCENE_NONE;
    g_game_data.running = false;
}

void TitleScene::OnSettingsButtonPressed() {
    return_scene = SCENE_SPLASH;
}