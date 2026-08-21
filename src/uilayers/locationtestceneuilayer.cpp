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



    CreateTextInput(size_x_input, start, input_size);
    CreateLabel(size_x_label, Vector2Add(size_x_input.position, h_spacing), 30, RAYWHITE, "size X");

    CreateTextInput(size_y_input,  Vector2Add(size_x_input.position, v_spacing), input_size);
    CreateLabel(size_y_label, Vector2Add(size_y_input.position, h_spacing), 30, RAYWHITE, "size Y");

    CreateTextInput(grid_size_input, Vector2Add(size_y_input.position, v_spacing), input_size);
    CreateLabel(grid_size_label, Vector2Add(grid_size_input.position, h_spacing), 30, RAYWHITE, "grid size");
    


}

LocationTestSceneUiLayer::~LocationTestSceneUiLayer() {
    //UnloadSound(button_sound);
    TraceLog(LOG_INFO, "UI DESTRUCTOR:  TITLE UI");
}

void LocationTestSceneUiLayer::Draw() {

    DrawLabelCentered(size_x_label, g_font);
    DrawTextInput(size_x_input, g_font);

    DrawLabelCentered(size_y_label, g_font);
    DrawTextInput(size_y_input, g_font);

    DrawLabelCentered(grid_size_label, g_font);
    DrawTextInput(grid_size_input, g_font);
}


void LocationTestSceneUiLayer::Update() {

    if(IsTextInputHovered(size_x_input, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        size_x_input.focussed = true;
    }
    else {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            size_x_input.focussed = false;
        }
    }

    if(IsTextInputHovered(size_y_input, g_viewport.scale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        size_y_input.focussed = true;
    }
    else {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            size_y_input.focussed = false;
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

    if(size_x_input.focussed) {
        int input_key = g_input.keys_pressed[0];
        int number = input_key - 48;
        if(number < 10 and number >= 0) {
            UpdateTextInput(size_x_input, input_key);
        }
        if(input_key == KEY_BACKSPACE) {
            UpdateTextInput(size_x_input, input_key);
        }

        if(input_key == KEY_ENTER and size_x_input.text != "") {
            int value = std::stoi(size_x_input.text);
            printf("%i\n", value);
            size_x_changed.EmitSignal();
            plan->size_x = value;
        }
    }

    if(size_y_input.focussed) {
        int input_key = g_input.keys_pressed[0];
        int number = input_key - 48;
        if(number < 10 and number >= 0) {
            UpdateTextInput(size_y_input, input_key);
        }
        if(input_key == KEY_BACKSPACE) {
            UpdateTextInput(size_y_input, input_key);
        }

        if(input_key == KEY_ENTER and size_y_input.text != "") {
            int value = std::stoi(size_y_input.text);
            printf("%i\n", value);
            size_y_changed.EmitSignal();
            plan->size_y = value;
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
            grid_size_changed.EmitSignal();
            plan->grid_size = value;
        }
    }
}
