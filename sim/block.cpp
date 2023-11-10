#include "block.hpp"
#include <cmath>

namespace Constants {
  // Information from initial file
  float ppm = 0.0;
  int np = 0.0;

  // Simulation scalar constants
  double radiusMultiplier = 1.695;
  double fluidDensity = pow(10, 3);
  double stiffnessPressure = 3.0;
  double stiffnessCollisions = 3 * pow(10, 4);
  double damping = 128.0;
  double viscosity = 0.4;
  double particleSize = 2 * pow(10, -4);
  double timeStep = pow(10, -3);

  // Simulation vector constants
  std::vector<double> externalAcceleration = {0.0, -9.8, 0.0};
  std::vector<double> boxUpperBound = {0.065, 0.1, 0.065};
  std::vector<double> boxLowerBound = {-0.065, -0.08, -0.065};

  // Simulation parameters
  double particleMass = fluidDensity / pow(ppm, 3);
  double smoothingLength = radiusMultiplier / ppm;
};
