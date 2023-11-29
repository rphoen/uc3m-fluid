# Fluid Simulation

## Introduction

Fluid flows simulation can be achieved by multiple methods. One of them is Smoothed Particle Hydrodynamics (SPH). SPH solves the Navier-Stokes equations at time steps. A fluid is modeled as a collection of particles. Those particles have interactions impacting their acceleration, position and
velocity.
Only particles at short distance have real interaction. To speedup simulation time, long distance interactions are discarded. To achieve this, the 3D space is divided in a grid of blocks and only particles in blocks in the nearby are considered.

Simulation incldes the following steps:
* Reading initial state of the simulation.
* Simulation of particles for each time step or iteration:
  * Repositioning of particles in the grid.
  * Computing forces and accelerations for each particle.
  * Processing particle collisions with boundaries.
  * Particles movement.
  * Reprocessing box boundaries interactions.
* Writing final state of the simulation.

## To build

Cmake was used to compile the program.
Clang-tidy was used to ensure the program follows safe coding practices. If you do not have clang-tidy installed, comment out this line in the CMakeLists.txt in the root folder:

```
set(CMAKE_CXX_CLANG_TIDY clang-tidy −header−filter=.∗)
```

Use these commands to initialize a cmake-build-debug folder and compile:

```
mkdir cmake-build-debug && cd cmake-build-debug && cmake ..
cmake --build cmake-build-debug
```

## To run

This program accepts 4 parameters in the CLI. 

For example, the command:
```
cmake-build-debug/fluid/fluid 2000 small.fld final.fld
```
Loads the file small.fld, runs 2000 time steps and geenrates an output file named final.fld. If the number of arguments is not exactly three arguments or contains invalid arguments, an error message will be generated.

