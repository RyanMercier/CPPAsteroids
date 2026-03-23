#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include "raylib.h"
#include "Config.h"
#include "Game.h"
#include "NetworkController.h"
#include "NeuralNetwork.h"
#include "Simulation.h"
#include "Population.h"

// Configuration defaults from Config.h
int rayCount = Config::Training::RAY_COUNT;
int numInputs = rayCount + Config::Training::EXTRA_INPUTS;
int numHidden = Config::Training::HIDDEN_NEURONS;
int numOutputs = Config::Training::NUM_OUTPUTS;
double mutationRate = Config::Training::MUTATION_RATE;
double crossoverRate = Config::Training::CROSSOVER_RATE;

int populationSize = Config::Training::POPULATION_SIZE;
int numGenerations = Config::Training::NUM_GENERATIONS;
int simsPerGeneration = Config::Training::SIMS_PER_GENERATION;
float simSpeed = Config::Training::SIM_SPEED;

bool drawBest = true;

std::string savePath = "./save.txt";
std::string csvPath = "./training_log.csv";

struct GenerationStats
{
    float best;
    float avg;
    float worst;
};

std::vector<GenerationStats> fitnessHistory;

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
    std::getline(file, line);
    double sFitness = std::stod(line.substr(9));

    std::getline(file, line);
    int sNumInputs = std::stoi(line);

    std::getline(file, line);
    int sNumHidden = std::stoi(line);

    std::getline(file, line);
    int sNumOutputs = std::stoi(line);

    std::getline(file, line);
    double sMutationRate = std::stod(line);

    std::getline(file, line);
    double sCrossoverRate = std::stod(line);

    std::vector<Neuron> hiddenLayer;
    std::getline(file, line);
    std::string neuronData = line.substr(1, line.length() - 2);

    std::istringstream hiddenLayerIss(neuronData);
    while (std::getline(hiddenLayerIss, neuronData, ')'))
    {
        hiddenLayer.push_back(ParseNeuron(neuronData));
    }

    std::vector<Neuron> outputLayer;
    std::getline(file, line);
    neuronData = line.substr(1, line.length() - 2);

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
    return fitness;
}

// Draws the fitness graph in the stats panel.
// X axis = generations, Y axis = auto-scaled fitness.
// Green = best, Yellow = average.
void DrawFitnessGraph(int panelX, int graphY, int graphW, int graphH)
{
    int pad = Config::Graph::PADDING;

    DrawRectangle(panelX, graphY, graphW, graphH, Color{20, 20, 20, 255});
    DrawRectangleLines(panelX, graphY, graphW, graphH, Color{60, 60, 60, 255});

    float maxFitness = 0.001f;
    for (const auto &s : fitnessHistory)
    {
        if (s.best > maxFitness) maxFitness = s.best;
    }

    int plotX = panelX + pad;
    int plotY = graphY + pad;
    int plotW = graphW - pad * 2;
    int plotH = graphH - pad * 2 - 20;

    if (fitnessHistory.size() >= 2)
    {
        for (size_t i = 1; i < fitnessHistory.size(); i++)
        {
            float x1 = plotX + ((float)(i - 1) / (numGenerations - 1)) * plotW;
            float x2 = plotX + ((float)i / (numGenerations - 1)) * plotW;

            float yBest1 = plotY + plotH - (fitnessHistory[i - 1].best / maxFitness) * plotH;
            float yBest2 = plotY + plotH - (fitnessHistory[i].best / maxFitness) * plotH;

            float yAvg1 = plotY + plotH - (fitnessHistory[i - 1].avg / maxFitness) * plotH;
            float yAvg2 = plotY + plotH - (fitnessHistory[i].avg / maxFitness) * plotH;

            DrawLine(x1, yBest1, x2, yBest2, GREEN);
            DrawLine(x1, yAvg1, x2, yAvg2, YELLOW);
        }
    }

    // Axis labels
    DrawText(TextFormat("%.1f", maxFitness), plotX + 2, plotY, 10, Color{100, 100, 100, 255});
    DrawText("0", plotX + 2, plotY + plotH - 10, 10, Color{100, 100, 100, 255});

    // Legend
    DrawLine(plotX, graphY + graphH - 14, plotX + 12, graphY + graphH - 14, GREEN);
    DrawText("Best", plotX + 16, graphY + graphH - 18, 10, GREEN);
    DrawLine(plotX + 60, graphY + graphH - 14, plotX + 72, graphY + graphH - 14, YELLOW);
    DrawText("Avg", plotX + 76, graphY + graphH - 18, 10, YELLOW);
}

// Draws the full stats panel to the right of the game area.
// Shows generation info, live sim stats, and the fitness graph.
void DrawStatsPanel(int generation, int simRound,
                    const std::unique_ptr<Simulation> &bestSim,
                    float lastBestFitness)
{
    int px = Config::Screen::WIDTH;
    int pw = Config::Panel::WIDTH;
    int pad = Config::Panel::PADDING;

    // Panel background
    DrawRectangle(px, 0, pw, Config::Screen::HEIGHT, Color{15, 15, 15, 255});
    DrawLine(px, 0, px, Config::Screen::HEIGHT, Color{40, 40, 40, 255});

    int x = px + pad;
    int y = pad;

    // Title
    DrawText("TRAINING", x, y, 24, RAYWHITE);
    y += 40;

    // Generation info
    DrawText(TextFormat("Generation  %d / %d", generation + 1, numGenerations), x, y, 16, LIGHTGRAY);
    y += 22;
    DrawText(TextFormat("Sim Round   %d / %d", simRound + 1, simsPerGeneration), x, y, 16, LIGHTGRAY);
    y += 22;
    DrawText(TextFormat("Population  %d", populationSize), x, y, 16, Color{100, 100, 100, 255});
    y += 30;

    // Sim speed slider
    DrawText(TextFormat("Sim Speed   %.1fx", simSpeed), x, y, 16, LIGHTGRAY);
    y += 22;

    int sliderW = pw - pad * 2;
    int sliderH = 12;
    float minSpeed = 0.1f;
    float maxSpeed = 20.0f;

    // Track background
    DrawRectangle(x, y, sliderW, sliderH, Color{40, 40, 40, 255});

    // Filled portion
    float t = (simSpeed - minSpeed) / (maxSpeed - minSpeed);
    DrawRectangle(x, y, (int)(sliderW * t), sliderH, Color{80, 140, 200, 255});

    // Handle click/drag on the slider
    Vector2 mouse = GetMousePosition();
    Rectangle sliderRect = {(float)x, (float)y, (float)sliderW, (float)sliderH};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, sliderRect))
    {
        float clickT = (mouse.x - x) / sliderW;
        if (clickT < 0) clickT = 0;
        if (clickT > 1) clickT = 1;
        simSpeed = minSpeed + clickT * (maxSpeed - minSpeed);
    }

    // Tick marks
    DrawText("0.1", x, y + sliderH + 2, 10, Color{60, 60, 60, 255});
    DrawText("20", x + sliderW - 14, y + sliderH + 2, 10, Color{60, 60, 60, 255});
    y += sliderH + 20;

    // Divider
    DrawLine(x, y, x + pw - pad * 2, y, Color{40, 40, 40, 255});
    y += 15;

    // Live stats from the best sim
    DrawText("BEST NETWORK", x, y, 18, RAYWHITE);
    y += 30;

    if (bestSim && bestSim->IsAlive())
    {
        DrawText(TextFormat("Kills        %d", bestSim->GetScore()), x, y, 16, GREEN);
        y += 22;
        DrawText(TextFormat("Time Alive   %.1f s", bestSim->GetLifeSpan()), x, y, 16, YELLOW);
        y += 22;
        DrawText(TextFormat("Shots Fired  %d", bestSim->GetShotsFired()), x, y, 16, LIGHTGRAY);
        y += 22;

        float currentFitness = CalculateFitness(bestSim);
        DrawText(TextFormat("Fitness      %.2f", currentFitness), x, y, 16, Color{100, 200, 255, 255});
        y += 30;
    }
    else
    {
        DrawText("Ship destroyed", x, y, 16, Color{200, 60, 60, 255});
        y += 30;
    }

    // Last generation's best fitness
    if (!fitnessHistory.empty())
    {
        DrawLine(x, y, x + pw - pad * 2, y, Color{40, 40, 40, 255});
        y += 15;

        DrawText("LAST GENERATION", x, y, 18, RAYWHITE);
        y += 28;
        DrawText(TextFormat("Best Fitness   %.2f", fitnessHistory.back().best), x, y, 16, GREEN);
        y += 22;
        DrawText(TextFormat("Avg Fitness    %.2f", fitnessHistory.back().avg), x, y, 16, YELLOW);
        y += 22;
        DrawText(TextFormat("Worst Fitness  %.2f", fitnessHistory.back().worst), x, y, 16, Color{200, 60, 60, 255});
        y += 35;
    }

    // Divider
    DrawLine(x, y, x + pw - pad * 2, y, Color{40, 40, 40, 255});
    y += 15;

    // Fitness graph
    DrawText("FITNESS OVER GENERATIONS", x, y, 14, LIGHTGRAY);
    y += 22;

    int graphW = pw - pad * 2;
    int graphH = Config::Graph::HEIGHT;
    DrawFitnessGraph(x, y, graphW, graphH);
    y += graphH + 20;

    // Config info at the bottom
    DrawLine(x, y, x + pw - pad * 2, y, Color{40, 40, 40, 255});
    y += 15;
    DrawText("CONFIG", x, y, 14, Color{80, 80, 80, 255});
    y += 20;
    DrawText(TextFormat("Rays: %d  Hidden: %d", rayCount, numHidden), x, y, 12, Color{60, 60, 60, 255});
    y += 16;
    DrawText(TextFormat("Mutation: %.2f  Crossover: %.2f", mutationRate, crossoverRate), x, y, 12, Color{60, 60, 60, 255});
}

void SaveCSV()
{
    std::ofstream csv(csvPath);
    if (csv.is_open())
    {
        csv << "generation,best_fitness,avg_fitness,worst_fitness" << std::endl;
        for (size_t i = 0; i < fitnessHistory.size(); i++)
        {
            csv << i + 1 << ","
                << fitnessHistory[i].best << ","
                << fitnessHistory[i].avg << ","
                << fitnessHistory[i].worst << std::endl;
        }
        csv.close();
        std::cout << "Training log saved to " << csvPath << std::endl;
    }
}

int main(int argc, char *argv[])
{
    bool useDefaults = false;
    bool loadFromFile = false;
    std::string loadPath = "";

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--default")
        {
            useDefaults = true;
        }
        else if (arg == "--load" && i + 1 < argc)
        {
            loadFromFile = true;
            loadPath = argv[++i];
        }
    }

    if (!useDefaults && !loadFromFile)
    {
        std::cout << "Load Network from File? (y/n): ";
        std::string loadInput;
        std::cin >> loadInput;
        loadFromFile = loadInput == "y" || loadInput == "Y";
    }

    if (!loadFromFile)
    {
        if (!useDefaults)
        {
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
        }

        numInputs = rayCount + Config::Training::EXTRA_INPUTS;

        std::cout << "Training with: rays=" << rayCount
                  << " hidden=" << numHidden
                  << " pop=" << populationSize
                  << " gens=" << numGenerations
                  << " sims/gen=" << simsPerGeneration
                  << " mutation=" << mutationRate
                  << " crossover=" << crossoverRate << std::endl;

        // Window is wider to fit the stats panel on the right
        int windowW = Config::Screen::WIDTH + Config::Panel::WIDTH;
        SetConfigFlags(FLAG_VSYNC_HINT);
        SetTraceLogLevel(LOG_ERROR);
        InitWindow(windowW, Config::Screen::HEIGHT, "Asteroids - Training");

        Population population = Population(populationSize, numInputs, numHidden, numOutputs, mutationRate, crossoverRate);

        float lastBestFitness = 0;

        for (int generation = 0; generation < numGenerations; generation++)
        {
            std::cout << "Computing Generation: " << generation + 1 << " / " << numGenerations << std::endl;

            for (int i = 0; i < populationSize; i++)
            {
                population.networks[i].fitness = 0;
            }

            for (int s = 0; s < simsPerGeneration; s++)
            {
                std::vector<std::unique_ptr<Simulation>> sims;

                // First sim draws to the window (ownsWindow=false, main owns it)
                // Pass &simSpeed so the slider can change speed live
                sims.push_back(std::make_unique<Simulation>(
                    std::make_unique<NeuralNetwork>(population.networks[0]),
                    rayCount, true, &simSpeed, false));

                for (int i = 1; i < populationSize; i++)
                {
                    sims.push_back(std::make_unique<Simulation>(
                        std::make_unique<NeuralNetwork>(population.networks[i]),
                        rayCount, false, &simSpeed));
                }

                // Main controls the frame: BeginDrawing -> game content -> panel -> EndDrawing
                bool alive = true;
                while (alive && !WindowShouldClose())
                {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    alive = false;
                    for (size_t j = 0; j < sims.size(); j++)
                    {
                        if (sims[j]->IsAlive())
                        {
                            sims[j]->Update();
                            alive = true;
                        }
                    }

                    // Draw the stats panel on the right side of the game
                    DrawStatsPanel(generation, s, sims[0], lastBestFitness);

                    EndDrawing();
                }

                for (int i = 0; i < populationSize; i++)
                {
                    float fitness = CalculateFitness(sims[i]);
                    population.networks[i].fitness += fitness;
                }
            }

            for (int i = 0; i < populationSize; i++)
            {
                population.networks[i].fitness /= simsPerGeneration;
            }

            // Collect generation stats
            NeuralNetwork bestNet = population.GetBestNet();
            float bestFit = bestNet.fitness;
            float totalFit = 0;
            float worstFit = bestFit;
            for (int i = 0; i < populationSize; i++)
            {
                float f = population.networks[i].fitness;
                totalFit += f;
                if (f < worstFit) worstFit = f;
            }
            float avgFit = totalFit / populationSize;

            fitnessHistory.push_back({bestFit, avgFit, worstFit});
            lastBestFitness = bestFit;

            std::cout << "Gen " << generation + 1
                      << " | Best: " << bestFit
                      << " | Avg: " << avgFit << std::endl;

            if (WindowShouldClose()) break;

            if (generation < numGenerations - 1)
            {
                population.Reproduce();
            }
        }

        NeuralNetwork bestNet = population.GetBestNet();
        std::cout << "Best Fitness: " << bestNet.fitness << std::endl;

        std::ofstream saveFile(savePath);
        if (saveFile.is_open())
        {
            saveFile << bestNet.ToString() << std::endl;
            saveFile.close();
            std::cout << "Best network saved to " << savePath << std::endl;
        }

        SaveCSV();
        CloseWindow();
    }
    else
    {
        if (loadPath.empty())
        {
            std::cout << "Enter File Path: ";
            std::cin >> loadPath;
        }
        std::cout << "Loading Network from " << loadPath << "..." << std::endl;
        std::ifstream file(loadPath);
        if (!file)
        {
            std::cout << "Error opening file" << std::endl;
            return 0;
        }

        NeuralNetwork net = LoadFromFile(loadPath);
        float loadSpeed = 1.0f;
        std::unique_ptr<Simulation> sim = std::make_unique<Simulation>(
            std::make_unique<NeuralNetwork>(net), rayCount, true, &loadSpeed);

        while (sim->IsAlive())
        {
            sim->Update();
        }
    }

    return 0;
}
