#include "systemobject/systemobject.hpp"
#include<raymath.h>




LocationLocalData GenerateLocationLocalData(int size) {

    LocationLocalData local_data;

    local_data.size = size;
    local_data.site_amount = std::sqrt(local_data.size) + 1;

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
    new_plan.grid_size = 20;
    new_plan.px_offset.x = new_plan.size_x * new_plan.grid_size * 0.5f; 
    new_plan.px_offset.y = new_plan.size_y * new_plan.grid_size * 0.5f;

    std::vector<int> temp_grid;
    temp_grid.resize(new_plan.size_x * new_plan.size_y, 0);

    for(int uid : local_data.site_uids) {
        Vector2 pos;
        pos.x = GetRandomValue(0, (new_plan.size_x - 2) - 1) + 1;
        pos.y = GetRandomValue(0, (new_plan.size_y - 2) - 1) + 1;

        new_plan.site_locations[uid] = pos *  new_plan.grid_size;
        //printf("new site %i  at grid %0.1f  %0.1f\n", uid, pos.x, pos.y); 
    }

    return new_plan;
}







SiteLocalData GenerateSiteLocalData() {

    SiteLocalData local_data;
    if(GetRandomValue(0, 100) < 50) {
        local_data.site_type = SITE_FUEL_SHOP;
    }

    return local_data;

}



SitePlan GenerateNewSitePlan(SiteLocalData local_data) {

    SitePlan new_plan;
    //local_data.size = 10;
    int x = std::sqrt(local_data.size);
    int y = std::sqrt(local_data.size);

    new_plan.size_x = x;
    new_plan.size_y = y;
    new_plan.grid_size = 10;
    new_plan.px_offset.x = new_plan.size_x * new_plan.grid_size * 0.5f; 
    new_plan.px_offset.y = new_plan.size_y * new_plan.grid_size * 0.5f;

    return new_plan;

}