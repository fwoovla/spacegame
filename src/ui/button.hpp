#pragma once
#include <raylib.h>
#include <string>
#include "../utils/utils.hpp"

struct Button {
    Vector2 position;
    Vector2 size;
    Vector2 center;
    Rectangle rect;
    Color focus_color;
    Color default_color;
    Color text_color_focus;
    Color text_color;
    std::string text;
    int text_size;
    bool hovered;
    bool already_hovered;
    bool clicked;
    Signal button_pressed;
};

void CreateButton(Button &_button, Vector2 _position, Vector2 _size, Color _focus_color, std::string _text);

void DrawButton(Button &_button);

bool IsButtonHovered(Button &_button, float _scale);