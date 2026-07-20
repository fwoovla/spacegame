#pragma once

#include <raylib.h>
#include <string>
#include "../uilayers/uilayers.hpp"
#include "../universe/universe.hpp"


enum SCENE_ID {
    SCENE_NONE,
    SCENE_SPLASH,
    SCENE_TITLE,
    SCENE_GAME,
};

class BaseScene {
    public:
    virtual ~BaseScene(){};
    virtual SCENE_ID Update() = 0;
    virtual void DrawScene() = 0;
    virtual void DrawUI() = 0;
    
    SCENE_ID scene_id = SCENE_SPLASH;
    SCENE_ID return_scene = SCENE_NONE;

    //MapData map_data;        
};


class SplashScene : public  BaseScene{
    public:
        SplashScene();
        ~SplashScene() override;
        SCENE_ID Update() override;
        void DrawScene() override;
        void DrawUI() override;
        void OnTimerTimeout();

        //Timer timer;
    };


class TitleScene : public  BaseScene{
    public:
        TitleScene();
        ~TitleScene() override;
        SCENE_ID Update() override;
        void DrawScene() override;
        void DrawUI() override;

        void OnStartButtonPressed();
        void OnExitButtonPressed();
        void OnSettingsButtonPressed();
        
        std::unique_ptr<TitleUiLayer> ui;
};





class LocationScene : public  BaseScene{
    public:
        LocationScene();
        ~LocationScene(){};
        SCENE_ID Update() override;
        void DrawScene() override;
        void DrawUI() override;

        void OnWorldTick();

        std::unique_ptr<GameUiLayer> ui;

        Timer world_ticker;

        LocationManager location_manager;

};


class GameScene : public  BaseScene{
    public:
        GameScene();
        ~GameScene() override;
        SCENE_ID Update() override;
        void DrawScene() override;
        void DrawUI() override;

        void OnWorldTick();
        void OnLandingRequested();
        void OnTakeoffRequested();


        
        std::unique_ptr<GameUiLayer> ui;

        Timer world_ticker;

        UniverseManager universe_manager;

        bool location_active = false;
        bool should_destroy_location = false;
        std::unique_ptr<LocationScene> location_scene;

};



/* void YSortEntities(World &world);

void YSortEntities(Chunk &chunk);

void YSortEntities(std::vector<BaseEntity *> draw_list); */