#include "uilayers.hpp"
#include "../game.h"

GameUiLayer::GameUiLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    CreateLabel(scene_label, top_center, 40, RAYWHITE, g_game_settings.title + "  GAME SCENE");

}

GameUiLayer::~GameUiLayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  GAME UI");
}

void GameUiLayer::Draw() {
    DrawLabelCenteredWithBG(scene_label, g_font, BLUE);
}

void GameUiLayer::Update() {

}