#include "shopui.hpp"




void MakeFuelShop(ShopData &shop_data) {
    printf("making fuel shop\n");
    for(auto &[id, fuel] : g_fuel_data) {
        ShopItem item = ShopItem();
        item.item_type = SHOP_ITEM_FUEL;
        item.item_id = fuel.fuel_type;
        item.price = fuel.price;

        CreateLabel(item.item_name_label, {}, 24, RAYWHITE, g_fuel_data[(FUEL_TYPE)item.item_id].name);

        shop_data.items.push_back(item);
    }

}

void MakeEquipmentShop(ShopData &shop_data) {
    printf("making equipment shop\n");

    for(auto &[id, e] : g_ship_equipment_data) {
        ShopItem item = ShopItem();
        item.item_type = SHOP_ITEM_EQUIPMENT;
        item.item_id = e->part_id;
        item.price = e->value;

        ShipEquipmentData *equipment_data = g_ship_equipment_data.at((SHIP_PART_ID)item.item_id);
        CreateLabel(item.item_name_label, {}, 24, RAYWHITE, equipment_data->name); 

        shop_data.items.push_back(item);
    }

}

