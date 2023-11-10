// fluid.cpp

#include <fstream>
#include <vector>

#include "block.hpp"

Fluid::Fluid(const std::string& input_file_name,
             const std::string& output_file_name) {
  // Read the input file.
  ReadInputFile(input_file_name);

  // Initialize the simulation constants.
  radiusMultiplier = 1.695;
  fluidDensity = 103.0;
  stiffnessPressure = 3.0;
  stiffnessCollision = 3.0e4;
  damping = 128.0;
  viscosity = 0.4;
  particleSize = 2.0e-4;
  timeStep = 1.0e-3;

  // Initialize the simulation box bounds.
  bmin_ = Eigen::Vector3d(-0.065, -0.08, -0.065);
  bmax_ = Eigen::Vector3d(0.065, 0.1, 0.065);
}

void Fluid::Simulate(int iterations) {
  for (int i = 0; i < iterations; i++) {
    SimulateStep();
  }
}

void Fluid::ReadInputFile(const std::string&
