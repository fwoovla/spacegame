    #include "uilayers.hpp"
    #include "../game.h"
    #include <string>
    
    
    


DebugUiLayer::DebugUiLayer() {

/*     Vector2 top_center = {g_game_data.window_settings.center.x, 20};

    
    Vector2 spacing = {0, 40};
    
    CreateLabel(header_label, {800, 20}, 50, RAYWHITE, g_app_settings.app_name + "  TITLE SCENE");
    CreateLabel(render_time_label, Vector2Add(header_label.position, spacing), 24, RAYWHITE, "  render time");
    CreateLabel(update_time_label, Vector2Add(render_time_label.position, spacing), 24, RAYWHITE, "  update time");
    CreateLabel(entity_count_label, Vector2Add(update_time_label.position, spacing), 24, RAYWHITE,  "  entity count");
    CreateLabel(tile_count_label, Vector2Add(entity_count_label.position, spacing), 24, RAYWHITE, "  tile count");
    CreateLabel(chunk_count_label, Vector2Add(tile_count_label.position, spacing), 24, RAYWHITE, "  chunk count");
    CreateLabel(fps_label, Vector2Add(chunk_count_label.position, spacing), 24, RAYWHITE, "  fps");

    CreateLabel(player_layer_label, {20, 20}, 40, RAYWHITE, g_app_settings.app_name + "  layer");
    CreateLabel(player_chunk_label, Vector2Add(player_layer_label.position, spacing), 40, RAYWHITE, "  chunk");
    CreateLabel(player_grid_label, Vector2Add(player_chunk_label.position, spacing), 40, RAYWHITE, "  grid");
    CreateLabel(player_world_label, Vector2Add(player_grid_label.position, spacing), 40, RAYWHITE, "  world");
    CreateLabel(imp, Vector2Add(player_world_label.position, spacing), 40, RAYWHITE, "  iso mouse");
    CreateLabel(wmp, Vector2Add(imp.position, spacing), 40, RAYWHITE, "  world mouse ");
    CreateLabel(selected_world_pos, Vector2Add(wmp.position, spacing), 40, RAYWHITE, "  selected world grid posision "); */


}


DebugUiLayer::~DebugUiLayer() {


}


void DebugUiLayer::Draw() {
/*     DrawLabelCentered(header_label, g_font);
    DrawLabelCentered(render_time_label, g_font);
    DrawLabelCentered(update_time_label, g_font);
    DrawLabelCentered(entity_count_label, g_font);
    DrawLabelCentered(tile_count_label, g_font);
    DrawLabelCentered(chunk_count_label, g_font);
    DrawLabelCentered(fps_label, g_font);
    DrawLabel(player_layer_label, g_font);
    DrawLabel(player_chunk_label, g_font);
    DrawLabel(player_grid_label, g_font);
    DrawLabel(player_world_label, g_font);
    DrawLabel(imp, g_font);
    DrawLabel(wmp, g_font);
    DrawLabel(selected_world_pos, g_font); */

}



void DebugUiLayer::Update() {
/* 
    render_time_label.text = "render time: \t" + std::to_string(g_game_data.debug_data.render_time) + "ms";
    update_time_label.text = "update time: \t" + std::to_string(g_game_data.debug_data.update_time) + "ms";
    entity_count_label.text = "entity count: \t" + std::to_string(g_game_data.debug_data.active_entities_count);
    tile_count_label.text = "tile count: \t" + std::to_string(g_game_data.debug_data.rendered_tiles_count);
    chunk_count_label.text = "chunk count: \t" + std::to_string(g_game_data.debug_data.active_chunks);
    fps_label.text = "fps: \t" + std::to_string(GetFPS());
    player_layer_label.text = "p_layer:  \t" + std::to_string((int)g_game_data.debug_data.player_layer) + "/" + std::to_string((int)g_game_data.debug_data.layer_count);
    player_chunk_label.text = "pc_pos:  \t" + std::to_string((int)g_game_data.debug_data.player_chunk_pos.x) + ", " + std::to_string((int)g_game_data.debug_data.player_chunk_pos.y);
    player_grid_label.text = "pg_pos:  \t" + std::to_string((int)g_game_data.debug_data.player_grid_pos.x) + ", " + std::to_string((int)g_game_data.debug_data.player_grid_pos.y);
    player_world_label.text = "pw_pos:  \t" + std::to_string((int)g_game_data.debug_data.player_world_pos.x) + ", " + std::to_string((int)g_game_data.debug_data.player_world_pos.y);

    imp.text = "imp:  \t" + std::to_string((int)g_game_data.debug_data.imp.x) + ", " + std::to_string((int)g_game_data.debug_data.imp.y);
    wmp.text = "wmp:  \t" + std::to_string((int)g_game_data.debug_data.wmp.x) + ", " + std::to_string((int)g_game_data.debug_data.wmp.y);
    selected_world_pos.text = "selected world pos:  \t" + std::to_string((int)g_game_data.debug_data.selected_world_pos.x) + ", " + std::to_string((int)g_game_data.debug_data.selected_world_pos.y);
 */
}
