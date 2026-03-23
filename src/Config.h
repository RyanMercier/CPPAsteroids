#ifndef CONFIG_H
#define CONFIG_H

namespace Config
{
    namespace Screen
    {
        constexpr int WIDTH = 1000;
        constexpr int HEIGHT = 1000;
    }

    namespace Panel
    {
        constexpr int WIDTH = 350;
        constexpr int PADDING = 20;
        // Total window width = Screen::WIDTH + Panel::WIDTH
    }

    namespace Ship
    {
        constexpr float BASE_SPEED = 20.0f;
        constexpr float MAX_SPEED = 500.0f;
        constexpr float ACCELERATION = 200.0f;
        constexpr float DRAG = 100.0f;
        constexpr float ROTATION_SPEED = 650.0f;
        constexpr float SHOTS_PER_SECOND = 10.0f;
        constexpr float START_ROTATION = 90.0f;
        constexpr float VELOCITY_STOP_THRESHOLD = 1.0f;
    }

    namespace Projectile
    {
        constexpr float SPEED = 700.0f;
        constexpr float RADIUS = 1.5f;
    }

    namespace Asteroid
    {
        constexpr float MIN_SPLIT_RADIUS = 30.0f;
        constexpr float COLLISION_SPLIT_MIN_RADIUS = 20.0f;
        constexpr float OFFSCREEN_BUFFER = 150.0f;
        constexpr float SPAWN_BUFFER = 100.0f;
        constexpr float SPAWN_EXCLUSION = 50.0f;
        constexpr double SPAWN_INTERVAL = 0.1;
        constexpr int INITIAL_MAX = 5;
        constexpr int DIFFICULTY_INTERVAL = 10;
        constexpr float COLLISION_PADDING = 2.0f;
        constexpr int POLY_BASE_SIDES = 5;
        constexpr float POLY_RADIUS_DIVISOR = 5.0f;
    }

    namespace Training
    {
        constexpr int RAY_COUNT = 12;
        constexpr int HIDDEN_NEURONS = 32;
        constexpr int NUM_OUTPUTS = 4;
        constexpr int EXTRA_INPUTS = 5; // rotation, pos x/y, vel x/y
        constexpr double MUTATION_RATE = 0.05;
        constexpr double CROSSOVER_RATE = 0.7;
        constexpr int POPULATION_SIZE = 500;
        constexpr int NUM_GENERATIONS = 200;
        constexpr int SIMS_PER_GENERATION = 10;
        constexpr float SIM_SPEED = 1.0f;
        constexpr float RAYCAST_MAX_DIST = 1000.0f;
    }

    namespace Network
    {
        constexpr float MUTATION_DELTA_SCALE = 0.3f;
        constexpr float WEIGHT_CLIP = 3.0f;
        constexpr int TOURNAMENT_SIZE = 5;
    }

    namespace Graph
    {
        constexpr int HEIGHT = 250;
        constexpr int PADDING = 5;
    }
}

#endif
