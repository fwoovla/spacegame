#include "uilayers.hpp"
#include "../game.h"
#include "../colordefs.hpp"

LocationTestSceneUiLayer::LocationTestSceneUiLayer() {

    Vector2 top_center = {g_viewport.resolution.x/2, 20.0f};
    printf("res = %0.5f, %0.5f --------------------------------------------------------------\n", g_viewport.resolution.x, g_viewport.resolution.y);

    Vector2 input_size = {200, 50};
    Vector2 input_pos = {50, 100};
    CreateLabel(size_x_label, input_pos, 40, RAYWHITE, "size X");
    CreateTextInput(size_x_input, input_pos, input_size);

    CreateLabel(size_x_label, {input_pos.x, input_pos.y + 50}, 40, RAYWHITE, "size Y");
    CreateTextInput(size_x_input, input_pos, input_size);

    CreateLabel(grid_size_label, {input_pos.x, input_pos.y + 100}, 40, RAYWHITE, "grid size");
    CreateTextInput(grid_size_input, input_pos, input_size);
    


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
        printf("input = %i\n", input_key);
        if(input_key != 0 and input_key != KEY_BACKSPACE) {
            char input_char = static_cast<char>(input_key);
            //printf("input char = %c\n\n", input_char);


            if(size_x_input.text.size() < size_x_input.max_chars) {
                size_x_input.text += input_char ;
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if(size_x_input.text.size() > 0) {
                size_x_input.text.pop_back();
            }
        }
    }
}
