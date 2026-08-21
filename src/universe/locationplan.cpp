#include "systemobject/systemobject.hpp"
#include<raymath.h>




LocationLocalData GenerateLocationLocalData(int size) {

    LocationLocalData local_data;

    local_data.size = size;
    local_data.site_amount = (local_data.size/400) + 1;

    for(int s = 0; s < local_data.site_amount; s++) {
        int uid = GetUID();
        local_data.site_uids.push_back(uid);
    }

    return local_data;

}


LocationPlan GenerateNewPlan(LocationLocalData local_data) {
    LocationPlan new_plan;

    int x = std::sqrt(local_data.size);
    int y = std::sqrt(local_data.size);

    new_plan.size_x = x;
    new_plan.size_y = y;
    new_plan.grid_size = 50;

    for(int uid : local_data.site_uids) {
        int _size = new_plan.size_y * new_plan.size_x;
        int index = GetRandomValue(0, _size - 1);
        Vector2 pos;
        pos.x = index % new_plan.size_y;
        pos.y = index / new_plan.size_y;

        new_plan.site_locations[uid] = pos;
        printf("new site %i  at grid %0.1f  %0.1f\n", uid, pos.x, pos.y);
    }


    return new_plan;
}