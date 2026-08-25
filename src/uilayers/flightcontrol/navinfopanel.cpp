#include "flightcontrol.hpp"


NavInfoPanel::NavInfoPanel(Vector2 position) {
    CreateLabel(distance_label, {position.x  + 10, position.y + 10}, 20, RAYWHITE, "target info");
    CreateLabel(info_label, {position.x  + 10, position.y + 40}, 20, RAYWHITE, "target info");
    
}


void NavInfoPanel::Draw() {
    if(shared_list_data->body != nullptr) {
        shared_list_data->target_position = shared_list_data->body->position;
        
    }
    else if(shared_list_data->location != nullptr) {
        shared_list_data->target_position = shared_list_data->location->position;
    }
    else if(shared_list_data->site != nullptr) {
        shared_list_data->target_position = shared_list_data->site->position;
    }
    else {
        shared_list_data->target_position = {0,0};
    }

    if(shared_list_data->target_position == (Vector2){0.0f,0.0f}) {
        distance_label.text = "N/A";
    }
    else {
        float distance_f = Vector2Distance(shared_list_data->this_position, shared_list_data->target_position);
        std::string  distance = TextFormat("%0.2f", distance_f);
        distance_label.text = distance + " km2";
    }

    DrawLabel(distance_label, g_font);
    DrawLabel(info_label, g_font);

}


void NavInfoPanel::UpdateInfo() {

    printf("info panel update!!\n");


    if(shared_list_data->body != nullptr) {
        std::string info = shared_list_data->body->name;
        info_label.text = info;
    }
    else if(shared_list_data->location != nullptr) {
        std::string info = shared_list_data->location->name;
        info_label.text = info;
    }
    else if(shared_list_data->site != nullptr) {
        std::string info = shared_list_data->site->name;
        info_label.text = info;
    }
    else {
        info_label.text = "N/A";
    }

    //printf("item selected from nav list\n");

}
