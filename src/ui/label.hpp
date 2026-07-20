#pragma once
#include <raylib.h>
#include <string>
#include <sstream>

struct Label {
    //Font *font;
    std::string text = "";
    Vector2 position = Vector2{0, 0};
    Color default_color = RAYWHITE;
    int text_size = 24;
};



void CreateLabel(Label &_label, Vector2 _position, int _text_size, Color _default_color, std::string _text);


void DrawLabelCentered(const Label &_label, const Font &font);


void DrawLabel(const Label &_label, const Font &font);

void DrawLabelWithBG(const Label &_label, const Font &font, Color color);

void DrawLabelCenteredWithBG(Label &_label, const Font &font, Color color);