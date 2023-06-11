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

    float mutationRate = 0.5f;
    float crossoverRate = 0.05f;

    float fitness = 0;

    std::vector<Neuron> hiddenLayer;
    std::vector<Neuron> outputLayer;

    // Constructor
    NeuralNetwork(int _inputs, int _hidden, int _outputs, float _mutationRate, float _crossoverRate)
        : numInputs(_inputs), numHidden(_hidden), numOutputs(_outputs), mutationRate(_mutationRate), crossoverRate(_crossoverRate) {}

    NeuralNetwork(int _inputs, int _hidden, int _outputs, float _mutationRate, float _crossoverRate, std::vector<Neuron> _hiddenLayer, std::vector<Neuron> _outputLayer)
        : numInputs(_inputs), numHidden(_hidden), numOutputs(_outputs), mutationRate(_mutationRate), crossoverRate(_crossoverRate), hiddenLayer(_hiddenLayer), outputLayer(_outputLayer) {}

    bool operator<(const NeuralNetwork &otherNet) const
    {
        return (fitness < otherNet.fitness);
    }

    std::string ToString()
    {
        std::string data = {
            "Fitness: " + std::to_string(fitness) + "\n" +
            std::to_string(numInputs) + "\n" +
            std::to_string(numHidden) + "\n" +
            std::to_string(numOutputs) + "\n" +
            std::to_string(mutationRate) + "\n" +
            std::to_string(crossoverRate) + "\n"};

        for (int i = 0; i < numHidden; i++)
        {
            data += "(" + hiddenLayer[i].ToString() + ")";
        }

        data += "\n";

        for (int i = 0; i < numOutputs; i++)
        {
            data += "(" + outputLayer[i].ToString() + ")";
        }

        return data;
    }

    // Initialize the neural network structure
    void
    initialize()
    {
        // Initialize hidden layer
        for (int i = 0; i < numHidden; i++)
        {
            std::vector<double> inputWeights(numInputs, randomWeight());
            double inputBias = randomBias();
            hiddenLayer.push_back(Neuron(inputWeights, inputBias));
        }

        // Initialize output layer
        for (int i = 0; i < numOutputs; i++)
        {
            std::vector<double> inputWeights(numHidden, randomWeight());
            double inputBias = randomBias();
            outputLayer.push_back(Neuron(inputWeights, inputBias));
        }
    }

    // Perform activation of the neural network
    std::vector<double> activate(const std::vector<double> &inputs)
    {
        std::vector<double> hiddenLayerOutputs;
        for (size_t i = 0; i < hiddenLayer.size(); i++)
        {
            hiddenLayer[i].activate(inputs);
            hiddenLayerOutputs.push_back(hiddenLayer[i].output);
        }

        std::vector<double> outputs;
        for (size_t i = 0; i < outputLayer.size(); i++)
        {
            outputLayer[i].activate(hiddenLayerOutputs);
            outputs.push_back(outputLayer[i].output);
        }

        return outputs;
    }

    // Genetic algorithm operations

    // Mutation operation
    void mutate()
    {
        for (size_t i = 0; i < hiddenLayer.size(); i++)
        {
            mutateNeuron(hiddenLayer[i]);
        }

        for (size_t i = 0; i < outputLayer.size(); i++)
        {
            mutateNeuron(outputLayer[i]);
        }
    }

    // Mutation operation on a neuron
    void mutateNeuron(Neuron &neuron)
    {
        for (size_t i = 0; i < neuron.inputWeights.size(); i++)
        {
            if (shouldMutate())
            {
                neuron.inputWeights[i] = randomWeight();
            }
        }

        if (shouldMutate())
        {
            neuron.inputBias = randomBias();
        }
    }

    // Crossover operation
    void crossover(NeuralNetwork &other)
    {
        for (size_t i = 0; i < hiddenLayer.size(); i++)
        {
            crossoverNeuron(hiddenLayer[i], other.hiddenLayer[i]);
        }

        for (size_t i = 0; i < outputLayer.size(); i++)
        {
            crossoverNeuron(outputLayer[i], other.outputLayer[i]);
        }
    }

    // Crossover operation on a neuron
    void crossoverNeuron(Neuron &neuron, const Neuron &other)
    {
        for (size_t i = 0; i < neuron.inputWeights.size(); i++)
        {
            if (shouldCrossover())
            {
                neuron.inputWeights[i] = other.inputWeights[i];
            }
        }

        if (shouldCrossover())
        {
            neuron.inputBias = other.inputBias;
        }
    }

    // Helper functions

    // Generate a random weight
    double randomWeight()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        return dis(gen);
    }

    // Generate a random bias
    double randomBias()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        return dis(gen);
    }

    // Determine if a mutation should occur based on mutation rate
    bool shouldMutate()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen) <= mutationRate;
    }

    // Determine if a crossover should occur based on crossover rate
    bool shouldCrossover()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen) <= crossoverRate;
    }
};

#endif