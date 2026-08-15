#include "flightcontrol.hpp"
#include "../../game.h"

ButtonPanel::ButtonPanel() {

}

void ButtonPanel::Update() {

    for(auto &p_button : buttons) {
        if(g_input.keys_pressed[0] == p_button.key) {
            button_payload->payload_i = p_button.button_index;
            button_payload->payload_s = p_button.payload_s;
            button_pressed.EmitSignal();
        }
    }

}

void ButtonPanel::Draw() {
    for(PanelButton &p_button : buttons) {
        DrawButton(p_button.button);  //the easiest button to button?
    }
}


void ButtonPanel::AddButton(Button new_button, KeyboardKey key, std::string payload_s) {
    PanelButton p_button;
    p_button.button = new_button;
    p_button.button_index = buttons.size();
    p_button.key = key;
    p_button.payload_s = payload_s;
    buttons.push_back(p_button);
    printf("adding button to panel %i\n", buttons.size());
}

void ButtonPanel::ClearButtons() {
    buttons.clear();

}