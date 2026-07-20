#include "textinput.hpp"
#include "../colordefs.hpp"


void CreateTextInput(TextInput &_TI, Vector2 _position, Vector2 size) {

    _TI.text = "";
    _TI.position = _position;
    _TI.text_box = {
        .x = _position.x,
        .y = _position.y,
        .width = size.x,
        .height = size.y
    };

}


bool IsTextInputHovered(TextInput &_TI, float _scale) {
    _TI.already_hovered = _TI.hovered;

    if(CheckCollisionPointRec(GetMousePosition(), _TI.text_box )) {
        _TI.hovered = true;
        return true;
    }
    _TI.hovered = false;
    return false;

}


void DrawTextInput(const TextInput &_TI, const Font &font) {

    Rectangle inner_rect = {
        .x = _TI.position.x + 2,
        .y = _TI.position.y + 2,
        .width = _TI.text_box.width - 4,
        .height = _TI.text_box.height - 4
    };


    Color outline_color = GRAY;
    Color bg_color = DARKERGRAY;

    if(_TI.hovered) {
        outline_color = RAYWHITE;
    }

    if(_TI.focussed) {
        outline_color = RAYWHITE;
        bg_color = _TI.bg_color;
    }

    DrawRectangleRec(_TI.text_box, outline_color);
    DrawRectangleRec(inner_rect, bg_color);

    Vector2 left_center = {
        .x = _TI.position.x + 10,
        .y = _TI.position.y + (_TI.text_box.height * 0.5f) - (_TI.text_size * 0.5f)
    };

    DrawTextEx(font, _TI.text.c_str(), left_center, _TI.text_size, 0, _TI.text_color);

}

void UpdateTextInput(TextInput &_TI, char new_input) {

    if(_TI.text.size() < _TI.max_chars) {
        _TI.text += new_input ;
    }

}