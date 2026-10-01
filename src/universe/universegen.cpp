#include "universe.hpp"



void UniverseGen_MakeGalaxy(UniverseData &universe_data) {

    universe_data.max_systems = 5000;
    universe_data.radius = 5000000.0f;
    universe_data.seed = GetRandomValue(0, 100000);
    SetRandomSeed(universe_data.seed);



    universe_data.galaxy_regions.resize(GALAXY_REGION_COUNT);
    for(int r = 0; r < GALAXY_REGION_COUNT; r++) {
        universe_data.galaxy_regions[r] =  UniverseGen_GenerateRegion((GALAXY_REGION_TYPE)r, universe_data.radius);
        
    }


}

GalaxyRegion UniverseGen_GenerateRegion(GALAXY_REGION_TYPE type, float galaxy_radius) {

    GalaxyRegion region;

    region.region_type = type;

    // ------------------------------------------------------------
    // CORE
    // ------------------------------------------------------------

    if(type == GALAXY_REGION_CORE) {

        region.radius = 0.2f;
        region.density = 0.9f;

        Circle circle;

        circle.center = { 0.0f, 0.0f };
        circle.radius = galaxy_radius * region.radius;

        region.circles.push_back(circle);
    }


    // ------------------------------------------------------------
    // BULGE
    // ------------------------------------------------------------

    if(type == GALAXY_REGION_BULDGE) {

        region.radius = 0.4f;
        region.density = 0.3f;

        Circle circle;

        circle.center = { 0.0f, 0.0f };
        circle.radius = galaxy_radius * region.radius;

        region.circles.push_back(circle);
    }


    // ------------------------------------------------------------
    // DISC
    // ------------------------------------------------------------

    if(type == GALAXY_REGION_DISC) {

        region.radius = 0.7f;
        region.density = 0.9f;

        Circle circle;

        circle.center = { 0.0f, 0.0f };
        circle.radius = galaxy_radius * region.radius;

        region.circles.push_back(circle);
    }


    // ------------------------------------------------------------
    // SPIRAL ARMS
    // ------------------------------------------------------------

    if(type == GALAXY_REGION_ARM) {

        region.radius = 1.0f;
        region.density = 0.1f;

        const int arm_count = 6;
        const int samples = 100;

        const float inner_radius =
            galaxy_radius * 0.7f;

        const float outer_radius =
            galaxy_radius * region.radius;

        const float spiral_turns = 0.15f;

        // Width of the arm.
        const float arm_radius =
            galaxy_radius * 0.06f;


        for(int arm = 0; arm < arm_count; arm++) {

            // Evenly distribute the arms around the galaxy.
            float start_angle =
                (2.0f * PI / arm_count) * arm;


            for(int i = 0; i < samples; i++) {

                float t =
                    (float)i / (float)(samples - 1);


                // Radius grows from the inner galaxy
                // toward the edge.
                float radius =
                    inner_radius +
                    (outer_radius - inner_radius) * t;


                // Angle increases as we move outward.
                float theta =
                    start_angle +
                    spiral_turns * 2.0f * PI * t;


                Vector2 position = {
                    cosf(theta) * radius,
                    sinf(theta) * radius
                };


                Circle circle;

                circle.center = position;
                circle.radius = arm_radius;


                region.circles.push_back(circle);
            }
        }
    }


    printf(
        "region type %i circles %zu\n",
        region.region_type,
        region.circles.size()
    );

    return region;
}

GALAXY_REGION_TYPE GetGalaxyRegion(const std::vector<GalaxyRegion> &regions, Vector2 position)
{
    for(int r = GALAXY_REGION_COUNT - 1; r >= 0; r--) {

        const GalaxyRegion& region = regions[r];

        for(const auto &circle : region.circles) {

            bool in_region = false;
            bool in_other_region = false;
            if(CheckCollisionPointCircle(position, circle.center, circle.radius)) {
                in_region = true;

                
            }
            if(in_region) {

                for(int _r = 0; _r < r; _r++) {
                    const GalaxyRegion& other_region = regions[_r];

                    for(const auto &_circle : other_region.circles) {
                        if(CheckCollisionPointCircle(position, _circle.center, _circle.radius)) {
                            in_other_region = true;
                            //return GALAXY_REGION_COUNT;
                        }
                    }
                }
            }

            //ok to add
            if(in_region and !in_other_region) {
                //if(GetRandomValue(0, 100) < region.density * 100) {
                    return region.region_type;
                //}
            }
        }
    }
    return GALAXY_REGION_COUNT;
}