#pragma once

#include <raylib.h>

struct LocationPlan {
    int size_x;
    int size_y;
    int grid_size;

};

LocationPlan GenerateNewPlan();