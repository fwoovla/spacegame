#include "uilayers.hpp"
#include "../game.h"
#include "../colordefs.hpp"

LocationTestSceneUiLayer::LocationTestSceneUiLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    printf("res = %0.5f, %0.5f --------------------------------------------------------------\n", g_viewport.resolution.x, g_viewport.resolution.y);

    Vector2 input_size = {100, 40};

    Vector2 start = {50, 100};

    Vector2 v_spacing = {0, 50};
    Vector2 h_spacing = {150, 25};



    CreateTextInput(size_input, start, input_size);
    //size_x_input.text = "0";
    CreateLabel(size_label, Vector2Add(size_input.position, h_spacing), 30, RAYWHITE, "size X");


    CreateTextInput(grid_size_input, Vector2Add(size_input.position, v_spacing), input_size);
    //grid_size_input.text = "0";
    CreateLabel(grid_size_label, Vector2Add(grid_size_input.position, h_spacing), 30, RAYWHITE, "grid size");

    CreateButton(regenerate_button, top_center, {200, 50}, RED, "regenerate location");
    


}

LocationTestSceneUiLayer::~LocationTestSceneUiLayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  TITLE UI");
}

void LocationTestSceneUiLayer::Draw() {

    DrawLabelCentered(size_label, g_font);
    DrawTextInput(size_input, g_font);

    DrawLabelCentered(grid_size_label, g_font);
    DrawTextInput(grid_size_input, g_font);

    DrawButton(regenerate_button);
}


void LocationTestSceneUiLayer::Update() {


    if(IsButtonHovered(regenerate_button, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        regenerate_button.clicked = true;
        regenerate_button.button_pressed.EmitSignal();

    }

    if(IsTextInputHovered(size_input, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        size_input.focussed = true;
    }
    else {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            size_input.focussed = false;
        }
    }


    if(IsTextInputHovered(grid_size_input, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        grid_size_input.focussed = true;
    }
    else {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            grid_size_input.focussed = false;
        }
    }

    if(size_input.focussed) {
        int input_key = g_input.keys_pressed[0];
        int number = input_key - 48;
        if(number < 10 and number >= 0) {
            UpdateTextInput(size_input, input_key);
        }
        if(input_key == KEY_BACKSPACE) {
            UpdateTextInput(size_input, input_key);
        }
        if(input_key == KEY_ENTER and size_input.text != "") {
            int value = std::stoi(size_input.text);
            printf("%i\n", value);
            //plan->size_x = value;
        }
    }



    if(grid_size_input.focussed) {
        int input_key = g_input.keys_pressed[0];
        int number = input_key - 48;
        if(number < 10 and number >= 0) {
            UpdateTextInput(grid_size_input, input_key);
        }
        if(input_key == KEY_BACKSPACE) {
            UpdateTextInput(grid_size_input, input_key);
        }
        if(input_key == KEY_ENTER and grid_size_input.text != "") {
            int value = std::stoi(grid_size_input.text);
            printf("%i\n", value);
            plan->grid_size = value;
        }
    }
}
