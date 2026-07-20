#pragma once

#include <memory>
#include "scenes.hpp"



class SceneManager {
    public:
    ~SceneManager();
    void Init();
    void UpdateScene();
    void DrawScene();
    void DrawUI();
    void ChangeSceneTo(SCENE_ID new_scene_id);

    //std::unique_ptr<BaseScene> current_scene;

    //DebugUiLayer debug_ui;

};


extern std::unique_ptr<BaseScene> g_current_scene;

