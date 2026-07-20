#include "resources.h"
#include "../game.h"

using json = nlohmann::json; 

void LoadSettings(std::string json_path) {

    

    std::ifstream cfile(json_path);
    if (!cfile.is_open()) {
        TraceLog(LOG_INFO, "CANNOT OPEN DATA FILE");
        return;
    }
    
    printf("loading settings\n");
    json j;
    cfile>>j;

    g_game_settings.title = j["title"];

}

void LoadResources() {
    g_font = LoadFontEx("assets/FFatF.ttf", 128, nullptr, 0);
    SetTextureFilter(g_font.texture, TEXTURE_FILTER_POINT);

}