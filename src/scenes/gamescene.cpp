#include "scenes.hpp"
#include "../game.h"
#include "../universe/entity.hpp"


GameScene::GameScene() {

    scene_id = SCENE_GAME;
    return_scene = SCENE_NONE;


    universe_manager.CreateUniverse(g_game_data.player_name);

    //universe_manager.current_system->landing_requested.Connect( [&]() { OnLandingRequested();});


    
    //EntityData player_data = GenerateEntityInstance(g_entity_template_data[ENTITY_PLAYER], 0, {500, 500});
    //universe_data.system_data.entity_data[player_data.uid] = player_data;
    

    g_viewport.map_width =  universe_manager.current_system->system_data.radius * 2;
    g_viewport.map_height =  universe_manager.current_system->system_data.radius * 2;

    g_camera = {0};
    g_camera.target = g_current_player->entity_data->position;
    g_camera.offset = {g_viewport.resolution.x * 0.5f, g_viewport.resolution.y * 0.5f};
    g_camera.rotation = 0.0f;
    g_camera.zoom = 2.0f;
    
    ui = std::make_unique<GameUiLayer>();

    world_ticker.Start(1.0f / g_game_data.tick_rate, false);
    world_ticker.timer_timeout.Connect( [&](){OnWorldTick();} );
    
    
    //HideCursor();
}


SCENE_ID GameScene::Update() {
    //return SCENE_NONE;
    return_scene = SCENE_NONE;
/* 

    if(should_destroy_location) {
        location_scene.reset();
        should_destroy_location = false;
        return return_scene;
    }

    if(location_active and location_scene != nullptr) {

        location_scene->Update();
        return return_scene;
    }
    
     */
    world_ticker.Update();

    HandleCamera();
 
    if(g_input.keys_pressed[0] == KEY_ENTER) {
        g_current_player->entity_data->position.x = (float)GetRandomValue(100, 20000);
        g_current_player->entity_data->position.y = (float)GetRandomValue(100, 20000);
    }


    //universe_manager.UpdateEntities();
    
    if(g_current_player != nullptr) {
        universe_manager.Update();
        //universe_manager.UpdateBodies(g_current_player->entity_data->position);
    }
 

    return return_scene;
}


void GameScene::DrawScene() {

    /* if(location_active and location_scene != nullptr) {

        location_scene->DrawScene();
        return;
    } */
    
    if(g_game_data.show_debug) {

        DrawCircleV(g_input.screen_mouse_position, 6, YELLOW);
    }

    BeginMode2D(g_camera);

    universe_manager.Draw();


    if(g_game_data.show_debug) {

        for(int orbit = 0; orbit < universe_manager.current_system->system_data.orbitals.size(); orbit++) {
            Vector2 sun_pos = universe_manager.current_system->system_data.star_position;
            float orbital =  universe_manager.current_system->system_data.orbitals[orbit];
            DrawCircleLines(sun_pos.x, sun_pos.y, orbital, WHITE);
        }
        for(int b = 0; b < universe_manager.current_system->system_data.body_list.size(); b++) {
            SystemBodyEntity &body = *universe_manager.current_system->system_data.body_list[b];
            DrawLineV(body.body_data->position, g_current_player->entity_data->position, GRAY);

            for(int sub_orbital = 0; sub_orbital < body.body_data->orbitals.size();sub_orbital++) {
                float orbital =  body.body_data->orbitals[sub_orbital];
                DrawCircleLines(body.body_data->position.x, body.body_data->position.y, orbital, YELLOW);   
            }
        }

        DrawRectangleLines(0, 0, universe_manager.current_system.get()->system_data.radius * 2, universe_manager.current_system.get()->system_data.radius * 2, WHITE);
        DrawCircleV(g_input.world_mouse_position, 4, GREEN);    
    }
    

    EndMode2D();

}


void GameScene::DrawUI() {

    universe_manager.DrawUI();
    ui->Draw();
    
}


GameScene::~GameScene() {
    //delete ui_layer;
    //world_manager.SaveWorld();
    TraceLog(LOG_INFO, "SCENE DESTRUCTOR:  GameScene");
}

void GameScene::OnWorldTick() {
    //printf("tick %i\n", g_game_data.tick_count);
    //g_game_data.tick_count++;
}







/* 
void GameScene::OnLandingRequested() {
    if(!location_active) {
        location_active = true;

        location_scene = std::make_unique<LocationScene>();

        int uid = g_current_player->entity_data->uid; 
       
        auto &old_data = universe_manager.current_system->system_data.entity_data[uid];

        location_scene->location_manager.location_data.entity_data[uid] = std::move(old_data);

        auto &new_data = location_scene->location_manager.location_data.entity_data[uid];

        universe_manager.current_system->system_data.entity_data.erase(uid);


        auto &system_entities = universe_manager.current_system->system_data.entity_list;

        for(auto it = system_entities.begin(); it != system_entities.end(); ++it)
        {
            if(it->get() == g_current_player)
            {
                

                // move ownership
                location_scene->location_manager.location_data.entity_list.push_back(std::move(*it));
                system_entities.erase(it);
                break;
            }
        } 

        g_current_player = dynamic_cast<PlayerCharacter*>(location_scene->location_manager.location_data.entity_list.back().get());
        g_current_player->entity_data = &new_data;

        g_current_player->entity_data->position = {0,0};

        g_camera.target = g_current_player->entity_data->position;

        location_scene->location_manager.launch_requested.Connect([&]() { OnTakeoffRequested();});
        //location_scene = std::make_unique<LocationScene>();

        printf("transition activated %i  %0.5f %0.5f\n", g_game_data.transition.location_id, g_game_data.transition.return_position.x, g_game_data.transition.return_position.y);
    }

}

void GameScene::OnTakeoffRequested() {

    if(!location_active)
        return;

    int uid = g_current_player->entity_data->uid;

    // Save return position before moving anything
    Vector2 return_position = g_game_data.transition.return_position;


    // Move entity data back to the system
    universe_manager.current_system->system_data.entity_data[uid] = std::move(location_scene->location_manager.location_data.entity_data[uid]);

    location_scene->location_manager.location_data.entity_data.erase(uid);


    // Move player entity ownership back
    auto &location_entities = location_scene->location_manager.location_data.entity_list;

    auto &system_entities = universe_manager.current_system->system_data.entity_list;


    for(auto it = location_entities.begin(); it != location_entities.end(); ++it)
    {
        if(it->get() == g_current_player)
        {
            system_entities.push_back(std::move(*it));
            location_entities.erase(it);
            break;
        }
    }


    // Re-acquire player pointer
    g_current_player = dynamic_cast<PlayerCharacter*>(system_entities.back().get());

    // Rebind entity data pointer
    g_current_player->entity_data = &universe_manager.current_system->system_data.entity_data[uid];


    // Restore position in system space
    g_current_player->entity_data->position = return_position;


    // Destroy location
    should_destroy_location = true;
    //location_scene.reset();
    location_active = false;


    // Reset camera
    g_camera.target = g_current_player->entity_data->position;


    printf("returned to system %f %f\n",
        return_position.x,
        return_position.y);
}

 */