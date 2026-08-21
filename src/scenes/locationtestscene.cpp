#include "scenes.hpp"
//#include "../app.hpp"

#define ZOOM_STEP 0.05f


LocationTestScene::LocationTestScene() {

    scene_id = SCENE_TEST;

    g_viewport.map_width =  10000;
    g_viewport.map_height =  10000;

    g_camera = {0};
    //g_camera.target = {-100,-100};
    g_camera.offset = {g_viewport.resolution.x * 0.5f, g_viewport.resolution.y * 0.5f};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 0.5f;  

    ui.plan = &plan;


    

    ui.regenerate_button.button_pressed.Connect([this](){OnRegenerateLocation();});

    //int size = GetRandomValue(5, 100);

    //local_data = GenerateLocationLocalData(size);

    OnRegenerateLocation();

}


SCENE_ID LocationTestScene::Update() {
    float zoom_factor = g_camera.zoom/1;

    float wheel_zoom = GetMouseWheelMove() * (ZOOM_STEP) * zoom_factor;
    g_camera.zoom += wheel_zoom;
    g_camera.target = GetMousePosition();

    return_scene = SCENE_NONE;

    if(IsKeyPressed(KEY_SPACE)) {
        //return_scene = SCENE_TITLE;
    }

    ui.Update();

    return return_scene;
}


void LocationTestScene::DrawScene() {

    BeginMode2D(g_camera);

    for(int y = 0; y <= plan.size_y; y++) {
        DrawLine(0, y * plan.grid_size, plan.size_x * plan.grid_size, y * plan.grid_size, WHITE);
        for(int x = 0; x <= plan.size_x; x++) {
            DrawLine( x * plan.grid_size, 0, x * plan.grid_size, plan.size_y * plan.grid_size, WHITE);
        }
    }

    for(auto &[uid, pos] : plan.site_locations) {
        DrawRectangle(pos.x * plan.grid_size, pos.y * plan.grid_size, plan.grid_size, plan.grid_size, BLUE);
    }

    EndMode2D();
    //printf("drawing test scene!!\n");

}


void LocationTestScene::DrawUI() {
    ui.Draw();
}


LocationTestScene::~LocationTestScene() {
    //delete ui_layer;
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  TestScene");
}


void LocationTestScene::OnSizeXChanged() {

}

void LocationTestScene::OnSizeYChanged() {

}

void LocationTestScene::OnGridSizeChanged() {

}

void LocationTestScene::OnRegenerateLocation() {
    printf("regenerate location\n");

    //int size = std::stoi(ui.size_input.text);
    int size = GetRandomValue(5, 1000);
    local_data = GenerateLocationLocalData(size);
    
    plan = GenerateNewPlan(local_data);
    
    ui.size_input.text =  std::to_string(local_data.size);
    ui.grid_size_input.text = std::to_string(plan.grid_size);
    printf("size: %i  #sites: %i\n", local_data.size, local_data.site_amount);
}

