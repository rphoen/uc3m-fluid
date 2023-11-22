#include "constants.hpp"
#include <iostream>

namespace Constants {
  // Simulation scalar constants
const double radiusMultiplier    = 1.695;
const double fluidDensity        = pow(10, 3);
const double stiffnessPressure   = 3.0;
const double stiffnessCollisions = 3 * pow(10, 4);
const double damping             = 128.0;
const double viscosity           = 0.4;
const double particleSize        = 2 * pow(10, -4);
const double timeStep            = pow(10, -3);

std::vector<std::vector<double>> simVectors;

const std::vector<double> externalAcceleration = simVectors[0];
const std::vector<double> boxUpperBound = simVectors[1];
const std::vector<double> boxLowerBound = simVectors[2];

  // Simulation vector constants
std::vector<std::vector<double>> initializeStatics() {
    std::vector<std::vector<double>> simConsVec;

    try {
    std::vector<double> const externalAccelerationLoose = {0.0, -9.8, 0.0};
    // const std::vector<float>& externalAcceleration = externalAccelerationLoose;
    simConsVec.emplace_back(externalAccelerationLoose);

    std::vector<double> const boxUpperBoundLoose = {0.065, 0.1, 0.065};
    //const std::vector<float>& boxUpperBound = boxUpperBoundLoose;
    simConsVec.emplace_back(boxUpperBoundLoose);

    std::vector<double> const boxLowerBoundLoose = {-0.065, -0.08, -0.065};
    //const std::vector<float>& boxLowerBound = boxLowerBoundLoose;
    simConsVec.emplace_back(boxLowerBoundLoose);

//    boxUpperBound        = {0.065, 0.1, 0.065};
//    boxLowerBound        = {-0.065, -0.08, -0.065};
  } catch (std::bad_alloc & e) {
    std::cerr << "Error: Memory allocation failed during initialization"
              << "\n";
    exit(1);
  }
  simVectors = simConsVec;
}
//const std::vector<double> externalAcceleration = {0.0, -9.8, 0.0};
//const std::vector<double> boxUpperBound = {0.065, 0.1, 0.065};
//const std::vector<double> boxLowerBound = {-0.065, -0.08, -0.065};

} // namespace Constants
