#include "uilayers.hpp"
#include "../game.h"
#include "../colordefs.hpp"

TitleUiLayer::TitleUiLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    printf("res = %0.5f, %0.5f --------------------------------------------------------------\n", g_viewport.resolution.x, g_viewport.resolution.y);

    CreateLabel(scene_label, top_center, 40, RAYWHITE, g_game_settings.title + "  TITLE SCENE");
    CreateLabel(version_label, {g_viewport.resolution.x - 100, g_viewport.resolution.y - 30}, 30, RAYWHITE, "Version 0.0.0");


    CreateButton(start_button, {top_center.x, top_center.y + 200}, {200, 50}, GREEN, "START");
    start_button.default_color = DARKGREEN;
    CreateButton(exit_button, {start_button.position.x, start_button.position.y + 50}, {200, 50}, RED, "EXIT");
    exit_button.default_color = DARKRED;
    CreateButton(settings_button, {exit_button.position.x, exit_button.position.y + 50}, {200, 50}, YELLOW, "SETTINGS");
    settings_button.default_color = DARKYELLOW;



    Vector2 input_size = {200, 50};
    Vector2 input_pos = {top_center.x - (input_size.x/2) , settings_button.position.y + 100};
    CreateLabel(name_label, {top_center.x, input_pos.y - 20}, 40, RAYWHITE, "Character Name");
    CreateTextInput(input, input_pos, input_size);
    input.text = g_game_data.player_name;

}

TitleUiLayer::~TitleUiLayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  TITLE UI");
}

void TitleUiLayer::Draw() {
    
    DrawLabelCenteredWithBG(scene_label, g_font, BLUE);
    DrawLabelCentered(version_label, g_font);

    DrawButton(start_button);
    DrawButton(exit_button);
    DrawButton(settings_button);

    DrawLabelCentered(name_label, g_font);
    DrawTextInput(input, g_font);
}


void TitleUiLayer::Update() {

    if(IsButtonHovered(start_button, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        start_button.clicked = true;
        g_game_data.player_name = input.text;
        start_button.button_pressed.EmitSignal();
    }

    if(IsButtonHovered(exit_button, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        exit_button.clicked = true;
        exit_button.button_pressed.EmitSignal();
    }

    if(IsButtonHovered(settings_button, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        settings_button.clicked = true;
        settings_button.button_pressed.EmitSignal();
    }

    if(IsTextInputHovered(input, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        input.focussed = true;
        //start_button.button_pressed.EmitSignal();
    }
    else {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            input.focussed = false;
        }
    }


    if(input.focussed) {
        int input_key = g_input.keys_pressed[0];
        printf("input = %i\n", input_key);
        if(input_key != 0 and input_key != KEY_BACKSPACE) {
            //std::string input_char = std::(input_key);
            char input_char = static_cast<char>(input_key);
            printf("input char = %c\n\n", input_char);


            if(input.text.size() < input.max_chars) {
                input.text += input_char ;
            }
            //UpdateTextInput(input, input_char);
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if(input.text.size() > 0) {
                input.text.pop_back();
            }
        }
    }
}
