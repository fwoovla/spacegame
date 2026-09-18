#pragma once
#include "../uilayers.hpp"


/* enum SHOP_TYPE {
    SHOP_NONE,
    SHOP_FUEL,
    SHOP_EQUIPMENT,
};
 */


 enum SHOP_ITEM_TYPE {
    SHOP_ITEM_FUEL,
    SHOP_ITEM_EQUIPMENT,
    //SHOP_ITEM_FOOD,
    //SHOP_ITEM_TOOL,
    //SHOP_ITEM_WEAPON,
};
 

class ShopItem : UILayer {
    public:
    ShopItem();
    ~ShopItem() {};
    void Update() override;
    void Draw() override;

    Label item_name_label;
    SHOP_ITEM_TYPE item_type = SHOP_ITEM_FUEL;
    int item_id = -1;
    float price = 0.0f;
    Vector2i grid_position;
    Rectangle bounds;
    bool hovered = false;
};



class ShopItemGrid : public UILayer {
    public:
    ShopItemGrid(){};
    ShopItemGrid(std::vector<ShopItem> *_items);
    ~ShopItemGrid() {};
    void Update() override;
    void Draw() override;

    std::vector<ShopItem> *items;
    Vector2i grid_size = {0,0};
    Vector2i item_size = {0,0};

};


struct ShopData {
    SHOP_TYPE shop_type = SHOP_NONE;
    std::string shop_name = "no shop name";
    std::vector<ShopItem> items;

};

class ShopUI : public UILayer {
    public:
    ShopUI();
    ~ShopUI(){};
    void Update() override;
    void Draw() override;

    void MakeShop(SHOP_TYPE s_type);

    ShopData shop_data;

    Label shop_name_label;

    ShopItemGrid item_grid;


    Button close_button;
    Signal close_shop;

};


//void MakeShop(ShopData &shop_data, SHOP_TYPE s_type);

void MakeFuelShop(ShopData &shop_data);

void MakeEquipmentShop(ShopData &shop_data);