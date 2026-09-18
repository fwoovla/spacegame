#include "shopui.hpp"


ShopItemGrid::ShopItemGrid(std::vector<ShopItem> *_items) {

    items = _items;

    item_size = {100, 100};
    grid_size = {4, 4};

    for(int index = 0; index < items->size(); index++) {
        int x = index % grid_size.x;
        int y = index / grid_size.x;

        items->at(index).grid_position = {x, y};
        items->at(index).bounds = {
            .x = (float)x*item_size.x,
            .y = (float)y*item_size.y,
            .width = (float)item_size.x,
            .height = (float)item_size.y,
        };
        
        items->at(index).item_name_label.position.x = items->at(index).bounds.x + (items->at(index).bounds.width/2);
        items->at(index).item_name_label.position.y = items->at(index).bounds.y + (items->at(index).bounds.height/2);
        
        printf("item grid position: %i %i\n", x, y);
    }
}

void ShopItemGrid::Update() {
    for(int index = 0; index < items->size(); index++) {
        items->at(index).Update();
    }
}


void ShopItemGrid::Draw() {

    for(int index = 0; index < items->size(); index++) {
        items->at(index).Draw();
    }
    
}