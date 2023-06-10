#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

class Neuron
{
public:
    std::vector<double> inputWeights;
    double inputBias;
    double output;

    // Constructor
    Neuron(std::vector<double> weights, double bias) : inputWeights(weights), inputBias(bias) {}

    // Activation function
    double activationFunction(double input)
    {
        return tanh(input); // 1 / (1 + exp(-input));
    }

    // Perform activation
    void activate(const std::vector<double> &inputs)
    {
        double sum = 0.0;
        for (size_t i = 0; i < inputs.size(); i++)
        {
            sum += inputs[i] * inputWeights[i];
        }
        output = activationFunction(sum + inputBias);
    }
};

#endif