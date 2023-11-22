#ifndef FLUID_CONSTANTS_HPP
#define FLUID_CONSTANTS_HPP

#include <cmath>
#include <vector>

namespace Constants {
// Simulation scalar constants
const double radiusMultiplier = 1.695;
const double fluidDensity = pow(10, 3);
const double stiffnessPressure = 3.0;
const double stiffnessCollisions = 3 * pow(10, 4);
const double damping = 128.0;
const double viscosity = 0.4;
const double particleSize = 2 * pow(10, -4);
const double timeStep = pow(10, -3);

// const std::vector<double> externalAcceleration = {0.0, -9.8, 0.0};
// const std::vector<double> boxUpperBound = {0.065, 0.1, 0.065};
// const std::vector<double> boxLowerBound = {-0.065, -0.08, -0.065};

inline const std::vector<double> &getExternalAcceleration() noexcept {
  static const std::vector<double> externalAcceleration = {0.0, -9.8, 0.0};
  return externalAcceleration;
}

inline const std::vector<double> &getBoxUpperBound() noexcept {
  static const std::vector<double> boxUpperBound = {0.065, 0.1, 0.065};
  return boxUpperBound;
}

inline const std::vector<double> &getBoxLowerBound() noexcept {
  static const std::vector<double> boxLowerBound = {-0.065, -0.08, -0.065};
  return boxLowerBound;
}

} // namespace Constants

#endif // FLUID_CONSTANTS_HPP
