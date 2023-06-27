#include "NeuralNetwork.h"

void NeuralNetwork::SetFitness(float _fitness)
{
    fitness = _fitness;
}

float NeuralNetwork::GetFitness()
{
    return fitness;
}

std::string NeuralNetwork::ToString()
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
void NeuralNetwork::Initialize()
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
std::vector<double> NeuralNetwork::activate(const std::vector<double> &inputs)
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
void NeuralNetwork::mutate()
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
void NeuralNetwork::mutateNeuron(Neuron &neuron)
{
    for (size_t i = 0; i < neuron.inputWeights.size(); i++)
    {
        if (shouldMutate())
        {
            float deltaWeight = randomWeight() * 0.1f;
            neuron.inputWeights[i] += deltaWeight;

            if (neuron.inputWeights[i] > 1.0f || neuron.inputWeights[i] < -1.0f)
            {
                neuron.inputWeights[i] -= 2.0f * deltaWeight;
            }
        }
    }

    if (shouldMutate())
    {
        float deltaBias = randomBias() * 0.1f;
        neuron.inputBias += deltaBias;

        if (neuron.inputBias > 1.0f || neuron.inputBias < -1.0f)
        {
            neuron.inputBias -= 2.0f * deltaBias;
        }
    }
}

// Crossover operation
void NeuralNetwork::crossover(NeuralNetwork &other)
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

// Crossover operation on a neuron using blending crossover
void NeuralNetwork::crossoverNeuron(Neuron &neuron, const Neuron &other)
{
    for (size_t i = 0; i < neuron.inputWeights.size(); i++)
    {
        if (shouldCrossover())
        {
            double alpha = 0.7; // Blend factor (adjust as needed)
            double newWeight = alpha * neuron.inputWeights[i] + (1 - alpha) * other.inputWeights[i];
            neuron.inputWeights[i] = newWeight;
        }
    }

    if (shouldCrossover())
    {
        double alpha = 0.7; // Blend factor (adjust as needed)
        double newBias = alpha * neuron.inputBias + (1 - alpha) * other.inputBias;
        neuron.inputBias = newBias;
    }
}

// Helper functions

// Generate a random weight
double NeuralNetwork::randomWeight()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    return dis(gen);
}

// Generate a random bias
double NeuralNetwork::randomBias()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    return dis(gen);
}

// Determine if a mutation should occur based on mutation rate
bool NeuralNetwork::shouldMutate()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) <= mutationRate;
}

// Determine if a crossover should occur based on crossover rate
bool NeuralNetwork::shouldCrossover()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) <= crossoverRate;
}