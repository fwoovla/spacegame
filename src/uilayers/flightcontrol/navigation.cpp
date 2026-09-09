#include "flightcontrol.hpp"
#include "../../game.h"


Navigation::Navigation(Rectangle min, Rectangle max) {

    min_bounds = min;
    max_bounds = max;

    system_list = SystemList({max_bounds.x + 50, max_bounds.y + 150});
    system_list.shared_list_data = &list_data;
    system_list.select_item.Connect( [this]() { OnSelectItem();} );
    

    CreateLabel(top_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 20}, 30, RAYWHITE, "Nav Target:");
    CreateLabel(nav_target_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 40}, 30, RAYWHITE, "target");
    CreateLabel(nav_distance_label, {(min_bounds.width/2) + min_bounds.x, min_bounds.y + 60}, 30, RAYWHITE, "distance");
    
    Button all_button;// = CreateButton(position, size, color, text)
    CreateButton(all_button, {max_bounds.x + 80, max_bounds.y + 90}, {20, 30}, ORANGE,"A");
    all_button.text_size = 20;
    all_button.text_color_focus = DARKERGRAY;
    all_button.text_color = GRAY;
    all_button.default_color = DARKRED;
    top_panel.AddButton(all_button, KEY_HOME, "ALL");

    Button planet_button;// = CreateButton(position, size, color, text)
    CreateButton(planet_button, {all_button.position.x + 30, all_button.position.y}, {20, 30}, ORANGE,"P");
    planet_button.text_size = 20;
    planet_button.text_color_focus = DARKERGRAY;
    planet_button.text_color = GRAY;
    planet_button.default_color = DARKRED;
    top_panel.AddButton(planet_button, KEY_DELETE, "BODIES");

    Button location_button;// = CreateButton(position, size, color, text)
    CreateButton(location_button, {planet_button.position.x + 30, all_button.position.y}, {20, 30}, ORANGE,"L");
    location_button.text_size = 20;
    location_button.text_color_focus = DARKERGRAY;
    location_button.text_color = GRAY;
    location_button.default_color = DARKRED;
    top_panel.AddButton(location_button, KEY_END, "LOCATIONS");

    Button site_button;// = CreateButton(position, size, color, text)
    CreateButton(site_button, {location_button.position.x + 30, all_button.position.y}, {20, 30}, ORANGE,"S");
    site_button.text_size = 20;
    site_button.text_color_focus = DARKERGRAY;
    site_button.text_color = GRAY;
    site_button.default_color = DARKRED;
    top_panel.AddButton(site_button, KEY_PAGE_DOWN, "SITES");

    top_panel.button_payload = &top_panel_payload;
    top_panel.button_pressed.Connect([this]() { OnTopPanelButtonPressed();});

    top_panel.buttons[0].toggled = true;



    info_panel = NavInfoPanel( {max_bounds.x + 320, max_bounds.y + 150} );
    info_panel.shared_list_data = &list_data;

    CreateButton(nav_button, {max_bounds.x + 440, max_bounds.y + max_bounds.height - 30}, {150, 30}, GREEN, "Set Target");
    nav_button.text_size = 20;
    nav_button.default_color = ORANGE;


    Button sys;
    CreateButton(sys, {max_bounds.x + 20, max_bounds.y + 150}, {40, 30}, GREEN, "S");
    sys.text_size = 20;
    sys.default_color = ORANGE;

    Button uni;
    CreateButton(uni, {max_bounds.x + 20, max_bounds.y + 180}, {40, 30}, GREEN, "U");
    uni.text_size = 20;
    uni.default_color = ORANGE;

    state_buttons[SYSTEM] = sys;
    state_buttons[UNIVERSE] = uni;

    universe_panel = UniversePanel( {0, 0, g_viewport.resolution.x, g_viewport.resolution.y - 120} );
    universe_panel.close_universe_panel.Connect([this]() {OnCloseUniversePanel();});
    universe_panel.set_system_target.Connect([this]() {OnSetSystemTarget();});

    /* universe_panel.universe_list.selected_system_data = &universe_panel.selected_system_data;
    universe_panel.universe_map.selected_system_data = &universe_panel.selected_system_data; */

    


}

Navigation::~Navigation() {

}

void Navigation::Update() {

    //printf("nav update??\n");
    if(shared_nav_data == nullptr) {
        return;
    }

    list_data.this_position = shared_nav_data->this_position;
    
    if(state == FOCUSED) {

        if(column == STATE and nav_state == SYSTEM) {
            nav_button.hovered = false;

            if(g_input.keys_pressed[0] == KEY_UP) {
                state_button_index = (NAV_STATE)(state_button_index - 1);
                if(state_button_index < 0) {
                    state_button_index = 1;
                }
            }
            if(g_input.keys_pressed[0] == KEY_DOWN) {
                state_button_index = (NAV_STATE)(state_button_index + 1);
            }
            state_button_index = (NAV_STATE)( state_button_index % 2 );


            for(int b = 0; b < state_buttons.size(); b++) {
                if(b == state_button_index) {
                    state_buttons[b].hovered = true;
                }
                else {
                    state_buttons[b].hovered = false;
                }
            }

            if(g_input.keys_pressed[0] == KEY_ENTER) {
                nav_state = (NAV_STATE)state_button_index;
                g_input.keys_pressed[0] = KEY_NULL;

            }
        }


        if(nav_state == SYSTEM) {

            top_panel.Update();
            system_list.Update(column == LIST);
            
            if(column == LIST) {
                nav_button.hovered = false;
            }
            else if (column == INFO) {
                nav_button.hovered = true;
                
                if(g_input.keys_pressed[0] == KEY_ENTER) {
                    deselect_nav_target.EmitSignal();
                    
                    *shared_nav_data = list_data;
                    shared_nav_data->set = true;
                    set_nav_target.EmitSignal();
                }
            }
            
            if(g_input.keys_pressed[0] == KEY_LEFT) {
                column = (NAV_COLUMN)( (column - 1) % 3 );
            }
            if(g_input.keys_pressed[0] == KEY_RIGHT) {
                column = (NAV_COLUMN)( (column + 1) % 3 );
            }
        }
        else if(nav_state == UNIVERSE) {
            universe_panel.Update();
        }
    }

    else if(state == MINIMIZED) {
        
    }
    

    if(shared_nav_data->site) {
        nav_target_label.text = shared_nav_data->site->name;
        shared_nav_data->target_position = shared_nav_data->site->position;
    }
    else if(shared_nav_data->location) {
        nav_target_label.text = shared_nav_data->location->name;
        shared_nav_data->target_position = shared_nav_data->location->position;
    }
    else if(shared_nav_data->body) {
        nav_target_label.text = shared_nav_data->body->name;
        shared_nav_data->target_position = shared_nav_data->body->position;
    }
    else {
        nav_target_label.text = "N/A";
    }

    if(shared_nav_data->set) {
        float distance_f = Vector2Distance(shared_nav_data->this_position, shared_nav_data->target_position);
        std::string  distance = TextFormat("%0.2f", distance_f);
        nav_distance_label.text = distance + " km2";
    }
    else {
        nav_distance_label.text = "N/A";
    }
    //printf("nav update!!\n");
}

void Navigation::Draw() {
  
    Rectangle bounds = min_bounds;
    Color color = RED;
    if(state == FOCUSED) {
        color = GREEN;
        bounds = max_bounds;
    }
    
    if(nav_state != UNIVERSE) {
        
        DrawRectangleRounded(bounds, 0.2f, 2, TRANSDARKERGRAY);
        DrawRectangleRoundedLines(bounds, 0.2f, 2, color);

        top_label.position = {(bounds.width/2) + bounds.x, bounds.y + 20};
        DrawLabelCentered(top_label, g_font);
        
        nav_target_label.position = {(bounds.width/2) + bounds.x, bounds.y + 40};
        DrawLabelCentered(nav_target_label, g_font);
        nav_distance_label.position = {(bounds.width/2) + bounds.x, bounds.y + 60};
        DrawLabelCentered(nav_distance_label, g_font);
    }

    if(state == FOCUSED) {
        if(nav_state == SYSTEM) {

            Rectangle sys_rec = {
                .x = max_bounds.x + 50,
                .y = max_bounds.y + 120,
                .width = 250.0f,
                .height = 240.0f
            };
            DrawRectangleRounded(sys_rec, .2, 10, BLACK);
            DrawRectangleRoundedLines(sys_rec, .2, 10, RAYWHITE);
            system_list.Draw({sys_rec.x + 20, sys_rec.y + 20}, column == LIST );
            
            top_panel.Draw();
            
            Rectangle info_rec = {
                .x = max_bounds.x + 320,
                .y = max_bounds.y + 120,
                .width = 250.0f,
                .height = 240.0f
            };
            DrawRectangleRounded(info_rec, .2, 10, BLACK);
            DrawRectangleRoundedLines(info_rec, .2, 10, RAYWHITE);
            info_panel.Draw();
               
            DrawButton(nav_button);

            for(auto &button : state_buttons) {
                if(column == STATE) {
                    button.focus_color = GREEN;
                    button.default_color = DARKGRAY;
                }
                else {
                    button.focus_color = ORANGE;
                    button.default_color = DARKGRAY;
                }
                DrawButton(button);
            }
        }
        else if(nav_state == UNIVERSE) {
            universe_panel.Draw();
        }      
    }
}

void Navigation::CreateSystemList(System *system) {

    system_list.master_nav_list.clear();

    for(auto &body : system->map_data.bodies) {
        NavListEntry new_body;
        new_body.body = &body.second;
        CreateLabel(new_body.label, {0,0}, 24, RAYWHITE, body.second.name);
        system_list.master_nav_list.push_back(new_body);
        //printf("new body\n");
        
    }

    for(auto &location : system->map_data.locations) {
        NavListEntry new_location;
        new_location.location = &location.second;
        CreateLabel(new_location.label, {0,0}, 24, RAYWHITE, location.second.name);
        system_list.master_nav_list.push_back(new_location);
        //printf("    new location\n");
    }

    for(auto &site : system->map_data.sites) {
        NavListEntry new_site;
        new_site.site = &site.second;
        CreateLabel(new_site.label, {0,0}, 24, RAYWHITE, site.second.name);
        system_list.master_nav_list.push_back(new_site);
        //printf("        new site\n\n");
    }

    system_list.new_list = true;

}


void Navigation::OnTopPanelButtonPressed() {
    printf("payload = %i   %s\n", top_panel_payload.payload_i, top_panel_payload.payload_s.c_str());

    for(auto & button : top_panel.buttons) {
        button.toggled = false;
    }

    if(top_panel_payload.payload_s == "ALL") {
        system_list.list_type = SystemList::LIST_TYPE::ALL;
        top_panel.buttons[SystemList::LIST_TYPE::ALL].toggled = true;
    }
    else if(top_panel_payload.payload_s == "BODIES") {
        system_list.list_type = SystemList::LIST_TYPE::BODIES;
        top_panel.buttons[SystemList::LIST_TYPE::BODIES].toggled = true;
    }
    else if(top_panel_payload.payload_s == "LOCATIONS") {
        system_list.list_type = SystemList::LIST_TYPE::LOCATIONS;
        top_panel.buttons[SystemList::LIST_TYPE::LOCATIONS].toggled = true;
    }
    else if(top_panel_payload.payload_s == "SITES") {
        system_list.list_type = SystemList::LIST_TYPE::SITES;
        top_panel.buttons[SystemList::LIST_TYPE::SITES].toggled = true;
    }
    system_list.new_list = true;
}


void Navigation::OnSelectItem() {
    printf("update info!!\n");
    info_panel.UpdateInfo();
    printf("update info!!\n");
}

void Navigation::OnCloseUniversePanel() {
    nav_state = SYSTEM;
    column = LIST;
    state_button_index = SYSTEM;

    for(int b = 0; b < state_buttons.size(); b++) {
        if(b == state_button_index) {
            state_buttons[b].hovered = true;
        }
        else {
            state_buttons[b].hovered = false;
        }
    }
}

void Navigation::SetSharedData(NavTargetSharedData *_shared_nav_data, int system_uid) {

/*     universe_panel.current_system_uid = system_uid;
    universe_panel.universe_list.current_system_uid = system_uid;
    universe_panel.universe_map.current_system_uid = system_uid;
 */
    universe_panel.universe_map.display_system_list = &universe_panel.universe_list.display_system_list;

    universe_panel.universe_list.selected_system_data = &universe_panel.selected_system_data;
    universe_panel.universe_map.selected_system_data = &universe_panel.selected_system_data;

    shared_nav_data = _shared_nav_data;

}

void Navigation::OnSetSystemTarget() {

    g_game_data.transition.system_id = universe_panel.selected_system_data.system->uid;

}

void Navigation::SetState(COMPONENT_STATE new_state) {
    state = new_state;
    if(new_state == COMPONENT_STATE::MINIMIZED) {
        nav_state = SYSTEM;
        column = LIST;
        state_button_index = SYSTEM;
    }
}