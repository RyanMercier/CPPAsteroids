#include "Population.h"

void Population::Reproduce()
{
    // Select parents for reproduction
    // Sort by fitness and keep the best half
    std::sort(networks.begin(), networks.end());
    std::vector<NeuralNetwork> newGeneration;

    // Move Elites to new generation
    for (int i = size / 4 + 1; i < size; i++)
    {
        newGeneration.push_back(networks[i]);
    }

    // Create new generation through crossover and mutation
    while (newGeneration.size() < size)
    {

        NeuralNetwork parent1 = networks[GetRandomInt(0, size)];
        NeuralNetwork parent2 = networks[GetRandomInt(0, size)];

        NeuralNetwork offspring1 = parent1;
        NeuralNetwork offspring2 = parent2;

        offspring1.crossover(parent2);
        offspring1.mutate();
        offspring2.crossover(parent1);
        offspring2.mutate();

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