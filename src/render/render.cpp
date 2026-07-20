#include "render.hpp"



void Render::Init(Vector2 resolution) {

    current_window.resolution = resolution;

    scene_render_texture = LoadRenderTexture(
        current_window.resolution.x, 
        current_window.resolution.y
    );

    ui_render_texture = LoadRenderTexture(
        current_window.resolution.x, 
        current_window.resolution.y
    );
        
    SetTextureFilter(scene_render_texture.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ui_render_texture.texture, TEXTURE_FILTER_BILINEAR);
    
    float scaleX = (float)GetScreenWidth() / current_window.resolution.x;
    float scaleY = (float)GetScreenHeight() / current_window.resolution.y;
    
    scale = (scaleX < scaleY) ? scaleX : scaleY;
    current_window.scale = scale;
    inv_scale = 1/scale;
    current_window.inv_scale = inv_scale;
    
    offsetX = (GetScreenWidth() - (current_window.resolution.x*scale)) / 2;
    offsetY = (GetScreenHeight() - (current_window.resolution.y*scale)) / 2;

}

void Render::DrawWindow(SceneManager &_sm) {
    //printf("drawing scene --renderer\n");

    BeginTextureMode(scene_render_texture);
    ClearBackground(BLANK);
        _sm.DrawScene();
    EndTextureMode();

        //ui
    BeginTextureMode(ui_render_texture);
    ClearBackground(BLANK);
        _sm.DrawUI();
    EndTextureMode();


    //-------------------------------------
    BeginDrawing();
    ClearBackground(BLANK);

        
    DrawTexturePro(
        scene_render_texture.texture,
        (Rectangle){ 0, 0, (float)scene_render_texture.texture.width, -(float)scene_render_texture.texture.height },
        (Rectangle){ (float)offsetX, (float)offsetY, current_window.resolution.x*scale, current_window.resolution.y*scale },
        (Vector2){0, 0}, 
        0.0f, 
        WHITE
    );
        
    DrawTexturePro(
        ui_render_texture.texture,
        (Rectangle){ 0, 0, (float)ui_render_texture.texture.width, -(float)ui_render_texture.texture.height },
        (Rectangle){ (float)offsetX, (float)offsetY, current_window.resolution.x*scale, current_window.resolution.y*scale },
        (Vector2){0, 0}, 0.0f, WHITE
    );


    EndDrawing();

    //printf("drawing window\n");

}



