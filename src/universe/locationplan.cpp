#include "systemobject/systemobject.hpp"
#include "../uilayers/shopui/shopui.hpp"
#include<raymath.h>




LocationLocalData GenerateLocationLocalData(int size) {

    LocationLocalData local_data;

    local_data.size = size;
    local_data.site_amount = std::sqrt(local_data.size);

    for(int s = 0; s < local_data.site_amount; s++) {
        int uid = GetUID();
        local_data.site_uids.push_back(uid);
    }

    return local_data;

}


LocationPlan GenerateNewLocationPlan(LocationLocalData local_data) {
    LocationPlan new_plan;

    int x = std::sqrt(local_data.size);
    int y = std::sqrt(local_data.size);

    new_plan.size_x = x;
    new_plan.size_y = y;
    new_plan.grid_size = 4;
    new_plan.px_offset.x = new_plan.size_x * new_plan.grid_size * 0.5f; 
    new_plan.px_offset.y = new_plan.size_y * new_plan.grid_size * 0.5f;

    std::vector<int> temp_grid;
    temp_grid.resize(new_plan.size_x * new_plan.size_y, 0);

    for(int uid : local_data.site_uids) {

        int grid_x;
        int grid_y;
        int index;

        int tries = 0;

        do {
            grid_x = GetRandomValue(1, new_plan.size_x - 2);
            grid_y = GetRandomValue(1, new_plan.size_y - 2);

            index = grid_y * new_plan.size_x + grid_x;
            tries++;
            if(tries > 5) {
                break;
            }

        } while(temp_grid[index] != 0);
        

        temp_grid[index] = uid;

        new_plan.site_locations[uid] = {
            (float)grid_x * new_plan.grid_size,
            (float)grid_y * new_plan.grid_size
        };
    }

    return new_plan;
}







SiteLocalData GenerateSiteLocalData() {

    SiteLocalData local_data;
    if(GetRandomValue(0, 100) < 50) {
        local_data.site_type = SITE_SHOP;
        local_data.shop_type = (SHOP_TYPE)GetRandomValue(0, SHOP_COUNT - 1);
    }
    else {
        local_data.site_type = SITE_LANDING;
    }

    return local_data;
}



SitePlan GenerateNewSitePlan(SiteLocalData local_data) {

    SitePlan new_plan;
    local_data.size = 10;
    int x = std::sqrt(local_data.size);
    int y = std::sqrt(local_data.size);

    new_plan.size_x = x;
    new_plan.size_y = y;
    new_plan.grid_size = 2;
    new_plan.px_offset.x = new_plan.size_x * new_plan.grid_size * 0.5f; 
    new_plan.px_offset.y = new_plan.size_y * new_plan.grid_size * 0.5f;

    return new_plan;

}