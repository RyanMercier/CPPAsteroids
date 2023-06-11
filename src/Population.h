#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include "NeuralNetwork.h"

class Population
{
    int size = 1;

public:
    std::vector<NeuralNetwork> networks;

    Population(int _size, int _numInputs, int _numHidden, int _numOutputs, double _mutationRate, double _crossoverRate)
    {
        size = _size;

        for (int i = 0; i < size; i++)
        {
            NeuralNetwork neuralNetwork(_numInputs, _numHidden, _numOutputs, _mutationRate, _crossoverRate);
            neuralNetwork.initialize();
            networks.push_back(neuralNetwork);
        }
    }

    double GetRandomInt(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return floor(dis(gen));
    }

    void Reproduce()
    {
        // Select parents for reproduction
        // Sort by fitness and keep the best half
        std::sort(networks.begin(), networks.end());
        std::vector<NeuralNetwork> parents;
        std::vector<NeuralNetwork> newGeneration;
        for (int i = size / 2; i < size; i++)
        {
            parents.push_back(networks[i]);
            newGeneration.push_back(networks[i]);
        }

        for (int i = 0; i < size; i++)
        {
            std::cout << networks[i].fitness << std::endl;
        }

        // Create new generation through crossover and mutation
        // Generate second half by mutating and crossing over the first half
        for (int i = 0; i < size / 2; i++)
        {

            NeuralNetwork parent1 = parents[GetRandomInt(0, size / 2)];
            NeuralNetwork parent2 = parents[GetRandomInt(0, size / 2)];

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

    NeuralNetwork GetBestNet()
    {
        std::sort(networks.begin(), networks.end());
        return networks.back();
    }
};

#endif