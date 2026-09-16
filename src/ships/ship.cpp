#include "ships.hpp"
#include "../game.h"

    Ship::Ship(ShipData *_data) {

    ship_data = _data;

    ship_controller = std::make_unique<ShipController>(_data);
    ship_controller->SetFlightMode(LOCAL_FLIGHT_MODE);
    //ship_controller->flight_modes[LOCAL_FLIGHT_MODE].velocity = {0,0};
    //ship_controller->flight_modes[LOCAL_FLIGHT_MODE].throttle = 0.0f;

    }

void Ship::Update(Vector2 &position) {
    ship_controller->Update(position);
}

void Ship::Draw(Vector2 &position, float scale) {

    Vector2 screen = GetWorldToScreen2D(position, g_camera);
    DrawCircleV(screen, ship_data->radius * scale, PINK);
    ship_controller->Draw(position, scale);

}


void Ship::UseFuel(int fuel_use) {
    ship_data->fuel -= fuel_use;

    if(ship_data->fuel < 0) {
        ship_data->fuel = 0;
    }
}

void Ship::Draw(Vector2 &position) {
    DrawCircleV({position.x, position.y}, 20, PINK);
}