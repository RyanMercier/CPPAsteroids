#include "Population.h"

void Population::Reproduce()
{
    // Select parents for reproduction
    // Sort by fitness and keep the best half
    std::sort(networks.begin(), networks.end());
    std::vector<NeuralNetwork> newGeneration;

    // Move Elites to new generation (top 25%)
    for (int i = size * 3 / 4; i < size; i++)
    {
        NeuralNetwork elite = networks[i];
        elite.fitness = 0;
        newGeneration.push_back(elite);
    }

    // Create new generation through crossover and mutation
    // Select parents from top half only (networks is sorted ascending)
    int topHalfStart = size / 2;
    while (newGeneration.size() < size)
    {
        NeuralNetwork parent1 = networks[(int)GetRandomInt(topHalfStart, size)];
        NeuralNetwork parent2 = networks[(int)GetRandomInt(topHalfStart, size)];

        NeuralNetwork offspring1 = parent1;
        NeuralNetwork offspring2 = parent2;

        offspring1.crossover(parent2);
        offspring1.mutate();
        offspring1.fitness = 0;
        offspring2.crossover(parent1);
        offspring2.mutate();
        offspring2.fitness = 0;

        newGeneration.push_back(offspring1);
        newGeneration.push_back(offspring2);
    }

    // Replace old population with new generation
    networks = newGeneration;
}

NeuralNetwork Population::GetBestNet()
{
    std::sort(networks.begin(), networks.end());
    return networks.back();
}

double Population::GetRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return floor(dis(gen));
}