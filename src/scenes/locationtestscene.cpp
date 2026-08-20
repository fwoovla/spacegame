#include "scenes.hpp"
//#include "../app.hpp"


LocationTestScene::LocationTestScene() {

    scene_id = SCENE_TEST;
}


SCENE_ID LocationTestScene::Update() {

    return_scene = SCENE_NONE;

    if(IsKeyPressed(KEY_SPACE)) {
        //return_scene = SCENE_TITLE;
    }

    ui.Update();

    return return_scene;
}


void LocationTestScene::DrawScene() {

    //printf("drawing test scene!!\n");

}


void LocationTestScene::DrawUI() {
    ui.Draw();
}


LocationTestScene::~LocationTestScene() {
    //delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TestScene");
}




