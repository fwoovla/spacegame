#include "universe.hpp"



void UniverseGen_MakeGalaxy(UniverseData &universe_data) {

    universe_data.max_systems = 500;
    universe_data.radius = 500000.0f;
    universe_data.seed = GetRandomValue(0, 100000);
    SetRandomSeed(universe_data.seed);



    universe_data.galaxy_regions.resize(GALAXY_REGION_COUNT);
    for(int r = 0; r < GALAXY_REGION_COUNT; r++) {
        universe_data.galaxy_regions[r] =  UniverseGen_GenerateRegion((GALAXY_REGION_TYPE)r);
        printf("region type\n");
    }


}

GalaxyRegion UniverseGen_GenerateRegion(GALAXY_REGION_TYPE type) {

    GalaxyRegion region;

    return region;


}