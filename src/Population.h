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
            neuralNetwork.Initialize();
            networks.push_back(neuralNetwork);
        }
    }

    ~Population()
    {
        networks.clear();
    }

    void Reproduce();

    NeuralNetwork GetBestNet();

    double GetRandomInt(int min, int max);
};

#endif