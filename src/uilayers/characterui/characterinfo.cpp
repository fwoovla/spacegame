#include "characterui.hpp"



CharacterInfo::CharacterInfo(Rectangle min, Rectangle max) {
    //ship = _ship;

    min_bounds = min;
    max_bounds = max;

    CreateLabel(top_label, {top_label.position.x, top_label.position.y + 30}, 30, RAYWHITE, "this character");

}

CharacterInfo::~CharacterInfo() {

}

void CharacterInfo::Update() {
    if(character == nullptr) {
        return;
    }

    entity_label.text = character->character_data->name;

}

void CharacterInfo::Draw() {
    
    Rectangle bounds = min_bounds;
    Color color = RED;
    if(state == FOCUSED) {
        color = GREEN;
        bounds = max_bounds;
    }
    DrawRectangleRounded(bounds, 0.2f, 2, TRANSDARKERGRAY);
    DrawRectangleRoundedLines(bounds, 0.2f, 2, color);

    top_label.position = {(bounds.width/2) + bounds.x, bounds.y + 20};
    DrawLabelCentered(top_label, g_font);


}

void CharacterInfo::SetState(COMPONENT_STATE new_state) {
    state = new_state;
}