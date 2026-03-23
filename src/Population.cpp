#include "Population.h"
#include "Config.h"

NeuralNetwork &Population::TournamentSelect()
{
    int bestIdx = (int)GetRandomInt(0, size);
    for (int i = 1; i < Config::Network::TOURNAMENT_SIZE; i++)
    {
        int idx = (int)GetRandomInt(0, size);
        if (networks[idx].fitness > networks[bestIdx].fitness)
        {
            bestIdx = idx;
        }
    }
    return networks[bestIdx];
}

void Population::Reproduce()
{
    std::sort(networks.begin(), networks.end());
    std::vector<NeuralNetwork> newGeneration;

    // Elites: keep top 5% unchanged
    int eliteCount = std::max(1, size / 20);
    for (int i = size - eliteCount; i < size; i++)
    {
        NeuralNetwork elite = networks[i];
        elite.fitness = 0;
        newGeneration.push_back(elite);
    }

    // Fill the rest via tournament selection + crossover + mutation
    while ((int)newGeneration.size() < size)
    {
        NeuralNetwork offspring = TournamentSelect();
        NeuralNetwork &parent2 = TournamentSelect();

        offspring.crossover(parent2);
        offspring.mutate();
        offspring.fitness = 0;

        newGeneration.push_back(offspring);
    }

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