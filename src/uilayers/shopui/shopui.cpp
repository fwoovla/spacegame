#include "shopui.hpp"
#include "../../game.h"




ShopUI::ShopUI() {
    CreateLabel(shop_name_label, {g_viewport.resolution.x/2, 50}, 30, RAYWHITE, "shop name");
    CreateButton(close_button, {g_viewport.resolution.x/2, g_viewport.resolution.y - 60}, {100, 30}, GREEN, "CLOSE");
}


void ShopUI::Update() {

    if(IsButtonHovered(close_button, g_viewport.scale) and g_input.mouse_left_down) {
        close_shop.EmitSignal();
    }

    shop_name_label.text = shop_data.shop_name;

    item_grid.Update();
}


void ShopUI::Draw() {
    Rectangle bounds = {
        20.0f,
        20.0f,
        g_viewport.resolution.x - 40,
        g_viewport.resolution.y - 40
    };

    DrawRectangleRounded(bounds, .5, 10, DARKERGRAY);
    DrawRectangleRoundedLines(bounds, .5, 10, RAYWHITE);
    DrawButton(close_button);
    DrawLabel(shop_name_label, g_font);

    item_grid.Draw();

}


void ShopUI::MakeShop(SHOP_TYPE s_type) {
    shop_data.items.clear();

    if(s_type == SHOP_FUEL) {
        shop_data.shop_name = "FUEL SHOP";
        MakeFuelShop(shop_data);
    }
    else if(s_type == SHOP_EQUIPMENT) {
        shop_data.shop_name = "EQUIPMENT SHOP";
        MakeEquipmentShop(shop_data);
    }

    shop_name_label.text = shop_data.shop_name;

    item_grid = ShopItemGrid(&shop_data.items);

}