#include "scenes.hpp"
#include "../game.h"

/* 
void YSortEntities(World & world) {
    
    world.world_entity_draw_list.clear();

    world.world_entity_draw_list.push_back(g_current_player);



    std::sort(world.world_entity_draw_list.begin(), world.world_entity_draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
    g_game_data.debug_data.active_entities_count = world.world_entity_draw_list.size();
    //TraceLog(LOG_INFO, "sorted entities  %i", world.world_draw_list.size());
}

void YSortEntities(Chunk &chunk) {
    
    chunk.draw_list.clear();

    Vector2i center_chunk =  {g_viewport.chunk_x, g_viewport.chunk_y};
    if(chunk.position == center_chunk) {
        chunk.draw_list.push_back(g_current_player);
    }

    for (auto &e : chunk.entity_list) {
        if(e->is_on_screen) {
            chunk.draw_list.push_back(e.get());
        }
    }

    std::sort(chunk.draw_list.begin(), chunk.draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
    //TraceLog(LOG_INFO, "sorted entities  %i", _map_data.draw_list.size());
}

void YSortEntities(std::vector<BaseEntity *> draw_list) {

    std::sort(draw_list.begin(), draw_list.end(),
    [](BaseEntity* a, BaseEntity* b) {
        return a->GetYSort() < b->GetYSort();
    });
} */