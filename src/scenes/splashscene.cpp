#include "scenes.hpp"
//#include "../app.hpp"


SplashScene::SplashScene() {

    scene_id = SCENE_SPLASH;
    //timer.timer_timeout.Connect( [&](){OnTimerTimeout();} );
    //timer.Start(3.0, true);
}


SCENE_ID SplashScene::Update() {

    return_scene = SCENE_NONE;

    //timer.Update();

    if(IsKeyPressed(KEY_SPACE)) {
        return_scene = SCENE_TITLE;
    }

    return return_scene;
}


void SplashScene::DrawScene() {

}


void SplashScene::DrawUI() {

}


SplashScene::~SplashScene() {
    //delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  SplashScene");
}


void SplashScene::OnTimerTimeout() {
    return_scene = SCENE_TITLE;
}

