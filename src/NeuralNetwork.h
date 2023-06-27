#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <string>
#include <random>
#include "Neuron.h"

class NeuralNetwork
{
public:
    int numInputs;
    int numHidden;
    int numOutputs;

    float mutationRate = 0.1f;
    float crossoverRate = 0.1f;

    float fitness = 0;

    std::vector<Neuron> hiddenLayer;
    std::vector<Neuron> outputLayer;

    // Constructor
    NeuralNetwork(int _inputs, int _hidden, int _outputs, float _mutationRate, float _crossoverRate)
        : numInputs(_inputs), numHidden(_hidden), numOutputs(_outputs), mutationRate(_mutationRate), crossoverRate(_crossoverRate) {}

    NeuralNetwork(int _inputs, int _hidden, int _outputs, float _mutationRate, float _crossoverRate, std::vector<Neuron> _hiddenLayer, std::vector<Neuron> _outputLayer)
        : numInputs(_inputs), numHidden(_hidden), numOutputs(_outputs), mutationRate(_mutationRate), crossoverRate(_crossoverRate), hiddenLayer(_hiddenLayer), outputLayer(_outputLayer) {}

    ~NeuralNetwork()
    {
    }

    bool operator<(const NeuralNetwork &otherNet) const
    {
        return (fitness < otherNet.fitness);
    }

    void SetFitness(float _fitness);

    float GetFitness();

    std::string ToString();

    // Initialize the neural network structure
    void Initialize();

    // Perform activation of the neural network
    std::vector<double> activate(const std::vector<double> &inputs);

    // Genetic algorithm operations

    // Mutation operation
    void mutate();

    // Mutation operation on a neuron
    void mutateNeuron(Neuron &neuron);

    // Crossover operation
    void crossover(NeuralNetwork &other);

    // Crossover operation on a neuron using blending crossover
    void crossoverNeuron(Neuron &neuron, const Neuron &other);

    // Helper functions

    // Generate a random weight
    double randomWeight();

    // Generate a random bias
    double randomBias();

    // Determine if a mutation should occur based on mutation rate
    bool shouldMutate();

    // Determine if a crossover should occur based on crossover rate
    bool shouldCrossover();
};

#endif