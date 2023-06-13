#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include "raylib.h"
#include "Game.h"
#include "NetworkController.h"
#include "NeuralNetwork.h"
#include "Simulation.h"
#include "Population.h"

int rayCount = 12;
int numInputs = rayCount + 5;
int numHidden = rayCount + 5;
int numOutputs = 4;
double mutationRate = 0.01;
double crossoverRate = 0.7;
int populationSize = 1;
int numGenerations = 10;

std::string savePath = "./save.txt";

Neuron ParseNeuron(const std::string &neuronData)
{
    std::string data = neuronData;
    data.erase(std::remove(data.begin(), data.end(), '('), data.end());
    data.erase(std::remove(data.begin(), data.end(), ')'), data.end());

    std::istringstream iss(data);
    std::string weight;
    std::getline(iss, weight, ',');
    double inputBias = std::stod(weight);

    std::vector<double> inputWeights;
    while (std::getline(iss, weight, ','))
    {
        inputWeights.push_back(std::stod(weight));
    }

    return Neuron(inputWeights, inputBias);
}

NeuralNetwork LoadFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);

    std::string line;
    std::getline(file, line); // Read fitness
    double sFitness = std::stod(line.substr(9));

    std::getline(file, line); // Read numInputs
    int sNumInputs = std::stoi(line);

    std::getline(file, line); // Read numHidden
    int sNumHidden = std::stoi(line);

    std::getline(file, line); // Read numOutputs
    int sNumOutputs = std::stoi(line);

    std::getline(file, line); // Read mutationRate
    double sMutationRate = std::stod(line);

    std::getline(file, line); // Read crossoverRate
    double sCrossoverRate = std::stod(line);

    std::vector<Neuron> hiddenLayer;
    std::getline(file, line);                                   // Read hiddenLayer line
    std::string neuronData = line.substr(1, line.length() - 2); // Remove parentheses

    std::istringstream hiddenLayerIss(neuronData);
    while (std::getline(hiddenLayerIss, neuronData, ')'))
    {
        hiddenLayer.push_back(ParseNeuron(neuronData));
    }

    std::vector<Neuron> outputLayer;
    std::getline(file, line);                       // Read outputLayer line
    neuronData = line.substr(1, line.length() - 2); // Remove parentheses

    std::istringstream outputLayerIss(neuronData);
    while (std::getline(outputLayerIss, neuronData, ')'))
    {
        outputLayer.push_back(ParseNeuron(neuronData));
    }

    for (int i = 0; i < hiddenLayer.size(); i++)
    {
        std::cout << hiddenLayer[i].ToString() << std::endl;
    }

    file.close();

    return NeuralNetwork(sNumInputs, sNumHidden, sNumOutputs, sMutationRate, sCrossoverRate, hiddenLayer, outputLayer);
}

int main(int argc, char *argv[])
{
    std::cout << "Begin Training ..." << std::endl;
    if (argc == 1)
    {
        // Create initial population
        Population population = Population(populationSize, numInputs, numHidden, numOutputs, mutationRate, crossoverRate);
        std::vector<std::unique_ptr<Simulation>> sims;

        // Evolutionary loop
        for (int generation = 0; generation < numGenerations; generation++)
        {
            std::cout << "Computing Generation: " << generation << " / " << numGenerations << std::endl;

            // Create simulations
            for (int i = 0; i < populationSize; i++)
            {
                sims.push_back(std::make_unique<Simulation>(std::make_unique<NeuralNetwork>(population.networks[i]), rayCount));
            }

            // Run sim and Evaluate fitness for each individual in the population
            bool alive = true;
            while (alive)
            {
                alive = false;
                for (int j = 0; j < sims.size(); j++)
                {
                    if (sims[j]->isAlive())
                    {
                        sims[j]->Update();
                        alive = true; // Set alive to true if any simulation is still alive
                    }
                }
            }

            // Clear previous simulations
            sims.clear();

            // GENETIC ALGORITHM
            population.Reproduce();
        }

        // Save best network
        NeuralNetwork bestNet = population.GetBestNet();
        std::cout << "Best Score of Generation: " << bestNet.fitness << std::endl;

        std::ofstream saveFile(savePath);
        if (saveFile.is_open())
        {
            saveFile << bestNet.ToString() << std::endl;
            saveFile.close();
        }
        else
        {
            std::cout << "Unable to open file";
        }
    }

    // Load Network
    else
    {
        std::ifstream file(argv[1]);
        if (!file)
        {
            // Handle file open error
            return 0;
        }

        NeuralNetwork net = LoadFromFile(argv[1]);
        std::unique_ptr<Simulation> sim = std::make_unique<Simulation>(std::make_unique<NeuralNetwork>(net), rayCount);

        while (sim->isAlive())
        {
            sim->Update();
        }
    }

    return 0;
}