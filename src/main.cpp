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

// Configuration
int rayCount = 12;
int numInputs = rayCount + 5;
int numHidden = 17;
int numOutputs = 4;
double mutationRate = 0.05;
double crossoverRate = 0.7;

int populationSize = 500;
int numGenerations = 200;
int simsPerGeneration = 10;
float simSpeed = 1.0f;

bool drawBest = true;

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

    std::cout << "Network Fitness: " << sFitness << std::endl;

    file.close();

    return NeuralNetwork(sNumInputs, sNumHidden, sNumOutputs, sMutationRate, sCrossoverRate, hiddenLayer, outputLayer);
}

float CalculateFitness(const std::unique_ptr<Simulation> &sim)
{
    float fitness = (sim->GetScore() + 1) * 10;
    fitness *= sim->GetLifeSpan();
    fitness *= sim->GetHitrate() * sim->GetHitrate();
    return fitness / 1000000;
}

int main(int argc, char *argv[])
{
    std::cout << "Load Network from File? (y/n): ";
    std::string loadInput;
    std::cin >> loadInput;
    bool loadFromFile = loadInput == "y" || loadInput == "Y";

    if (!loadFromFile)
    {
        // Specify network properties
        std::cout << "Enter Number of Raycasts: ";
        std::cin >> rayCount;

        std::cout << "Enter Number of Hidden Neurons: ";
        std::cin >> numHidden;

        std::cout << "Enter Mutation Rate as Decimal from 0.0 to 1.0: ";
        std::cin >> mutationRate;

        std::cout << "Enter Crossover Rate as Decimal from 0.0 to 1.0: ";
        std::cin >> crossoverRate;

        std::cout << "Enter Population Size: ";
        std::cin >> populationSize;

        std::cout << "Enter Number of Generations: ";
        std::cin >> numGenerations;

        std::cout << "Enter Number of Simulations per Generation: ";
        std::cin >> simsPerGeneration;

        std::cout << "Enter Simulation Speed: ";
        std::cin >> simSpeed;

        // std::cout << "Draw Best Simulation? (y/n): ";
        // std::string drawBestInput;
        // std::cin >> drawBestInput;
        // drawBest = drawBestInput == "y" || drawBestInput == "Y";

        std::cout << "Begin Training ..." << std::endl;
        // Create initial population
        Population population = Population(populationSize, numInputs, numHidden, numOutputs, mutationRate, crossoverRate);

        // Evolutionary loop
        for (int generation = 0; generation < numGenerations; generation++)
        {
            std::cout << "Computing Generation: " << generation + 1 << " / " << numGenerations << std::endl;

            // Run multiple simulations per generation to mitigate luck
            for (int i = 0; i < simsPerGeneration; i++)
            {
                std::vector<std::unique_ptr<Simulation>> sims;
                sims.push_back(std::make_unique<Simulation>(std::make_unique<NeuralNetwork>(population.networks[0]), rayCount, drawBest, simSpeed));

                // Create simulations
                for (int i = 1; i < populationSize; i++)
                {
                    sims.push_back(std::make_unique<Simulation>(std::make_unique<NeuralNetwork>(population.networks[i]), rayCount, false, simSpeed));
                }

                // Run simulations and evaluate fitness
                bool alive = true;
                while (alive)
                {
                    alive = false;
                    for (int j = 0; j < sims.size(); j++)
                    {
                        if (sims[j]->IsAlive())
                        {
                            sims[j]->Update();
                            alive = true; // Set alive to true if any simulation is still alive
                        }
                    }
                }

                // Update fitness values in the population
                for (int i = 0; i < populationSize; i++)
                {
                    float fitness = CalculateFitness(sims[i]);
                    population.networks[i].fitness += fitness;
                }
            }

            // Save best network
            NeuralNetwork bestNet = population.GetBestNet();
            std::cout << "Highest Fitness of Generation: " << bestNet.fitness << std::endl;

            if (generation <= numGenerations - 1)
            {
                // Genetic algorithm
                population.Reproduce();
            }
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
            std::cout << "Unable to open file" << std::endl;
        }
    }

    // Load network
    else
    {
        std::cout << "Enter File Path: ";
        std::string filePath;
        std::cin >> filePath;
        std::cout << "Loading Network..." << std::endl;
        std::ifstream file(filePath);
        if (!file)
        {
            std::cout << "Error opening file" << std::endl;
            return 0;
        }

        NeuralNetwork net = LoadFromFile(filePath);
        std::unique_ptr<Simulation> sim = std::make_unique<Simulation>(std::make_unique<NeuralNetwork>(net), rayCount, true, 1.0f);

        while (sim->IsAlive())
        {
            sim->Update();
        }
    }

    return 0;
}