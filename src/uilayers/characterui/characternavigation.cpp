#include "characterui.hpp"

#include "../../game.h"


CharacterNavigation::CharacterNavigation(Rectangle min, Rectangle max) {

    min_bounds = min;
    max_bounds = max;


    /* universe_panel.universe_list.selected_system_data = &universe_panel.selected_system_data;
    universe_panel.universe_map.selected_system_data = &universe_panel.selected_system_data; */

    


}

CharacterNavigation::~CharacterNavigation() {

}

void CharacterNavigation::Update() {

    //printf("nav update??\n");
/*     if(shared_nav_data == nullptr) {
        return;
    }
 */

    //printf("nav update!!\n");
}

void CharacterNavigation::Draw() {
  
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

void CharacterNavigation::SetState(COMPONENT_STATE new_state) {
    state = new_state;
}

/* */
