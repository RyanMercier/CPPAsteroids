# CPPAsteroids

A neuroevolution system that trains neural networks to play Asteroids. Ships learn to dodge asteroids and shoot through a genetic algorithm, starting from completely random behavior and converging on survival strategies over generations.

Built with C++ and raylib.

## Building

### Dependencies

You need raylib installed. On Ubuntu/WSL:

```bash
sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev cmake git

cd /tmp && git clone --depth 1 https://github.com/raysan5/raylib.git
cd raylib && mkdir build && cd build
cmake .. && make -j$(nproc) && sudo make install
sudo ldconfig
```

### Compile

```bash
make
```

## Usage

### Train with default settings

```bash
./main --default
```

This runs training with: 12 raycasts, 32 hidden neurons, population of 500, 200 generations, 10 sims per generation, 5% mutation rate, 70% crossover rate. All defaults are defined in `src/Config.h`.

### Train with custom settings

```bash
./main
```

Running without flags drops you into interactive mode where you configure every parameter.

### Load a saved network

```bash
./main --load save.txt
```

Loads a previously trained network and runs it visually so you can watch it play.

## How it works

### Network architecture

Each ship is controlled by a feedforward neural network with one hidden layer (32 neurons by default, configurable). The network takes 17 inputs:

- Ship rotation, position (x, y), and velocity (x, y)
- 12 raycasts evenly spaced around the ship, measuring distance to the nearest asteroid in each direction

Raycasts are relative to the ship's facing direction, so the network sees the world ego-centrically. All inputs are normalized to [-1, 1].

The 4 outputs map directly to controls: thrust, turn left, turn right, shoot. Each output is thresholded at 0.

### Genetic algorithm

A population of networks is evaluated over multiple simulation rounds per generation to reduce the effect of luck. Fitness is based on score, survival time, and shot accuracy (kills per shot fired), then averaged across rounds.

After evaluation, the top 25% of networks survive as elites. New offspring are produced by selecting parents from the top 50% and applying blended crossover and small random mutations to their weights and biases.

### Fitness function

```
fitness = (score + 1) * 10 * lifespan * accuracy^2
```

Where accuracy is `kills / shots_fired`. This rewards networks that survive long, score kills, and don't waste ammo. Networks that spray-fire get penalized because their accuracy drops.

## Training UI

The training window is split into two panels. The left side shows the best network of each generation playing in real time. The right side is a stats panel with:

- Current generation and sim round
- Sim speed slider (0.1x to 20x, click to drag)
- Live stats for the best network: kills, time alive, shots fired, fitness
- Last generation's best, average, and worst fitness
- A fitness graph plotting best (green) and average (yellow) fitness across all generations

The best network is saved to `save.txt` when training finishes. A CSV log (`training_log.csv`) is also written with per-generation fitness stats for further analysis or plotting.

## Configuration

All game constants, physics values, and training defaults live in `src/Config.h`. Screen dimensions, ship speed, asteroid behavior, network hyperparameters, and graph layout are all defined there.
