#pragma once

#include <raylib.h>
#include <string>

struct TextInput {

    std::string text = "";
    Vector2 position = Vector2{0, 0};
    Color text_color = RAYWHITE;
    Color bg_color = DARKGRAY;
    int text_size = 36;
    Rectangle text_box;
    bool focussed = false;
    bool hovered = false;
    bool already_hovered = false;
    int max_chars = 20;
};



void CreateTextInput(TextInput &_TI, Vector2 _position, Vector2 size);

bool IsTextInputHovered(TextInput &_TI, float _scale);

void DrawTextInput(const TextInput &_TI, const Font &font);

void UpdateTextInput(TextInput &_TI, char new_input);

