#pragma once
#include "../uilayers.hpp"

class UILayer{
    public:
    virtual ~UILayer(){};
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class FuelShop : public UILayer {

    public:
    ~FuelShop(){};
    void Update();
    void Draw();
};


class ShipPartShop : public UILayer {

    public:
    ~ShipPartShop(){};
    void Update();
    void Draw();;
};