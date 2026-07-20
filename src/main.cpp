#include "resources/resources.h"
#include "game.h"

GameSettings g_game_settings;

int main(int argc, char *argv[]){


    LoadSettings("assets/settings.json");


    InitWindow( RES_X, RES_Y, g_game_settings.title.c_str());

    SetTargetFPS(60);

    Game game;

    game.StartGame();

    game.CleanUpGame();

    CloseWindow();

    return 0;
}