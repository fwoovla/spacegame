#include "resources/resources.h"
#include "game.h"

GameSettings g_game_settings;

int main(int argc, char *argv[]){

    bool fullscreen = false;
    for (int i = 1; i < argc; ++i) {
        std::string param = argv[i];
        printf("arg = %s\n", param.c_str());
        if(param == "-F") {
            fullscreen = true;
        }
    }

    if(fullscreen) {SetConfigFlags(FLAG_FULLSCREEN_MODE);}

    LoadSettings("assets/settings.json");

    //SetConfigFlags(FLAG_FULLSCREEN_MODE);

    InitWindow( RES_X, RES_Y, g_game_settings.title.c_str());

    SetTargetFPS(60);

    Game game;

    game.StartGame();

    game.CleanUpGame();

    CloseWindow();

    return 0;
}