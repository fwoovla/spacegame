#include "shopui.hpp"
#include "../../game.h"



ShopItem::ShopItem() {

}


void ShopItem::Update() {

    if(CheckCollisionPointRec(g_input.screen_mouse_position, bounds)) {
        hovered = true;
    }
    else {
        hovered = false;
    }

}

void ShopItem::Draw() {

    Color color = GRAY;
    if(hovered) {
        color = RAYWHITE;
    }
    
    DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, color);
    DrawLabelCentered(item_name_label, g_font);

}