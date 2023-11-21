#ifndef FLUID_CONSTANTS_HPP
#define FLUID_CONSTANTS_HPP

#include <cmath>
#include <vector>

namespace Constants {
// Simulation scalar constants
extern const double radiusMultiplier;
extern const double fluidDensity;
extern const double stiffnessPressure;
extern const double stiffnessCollisions;
extern const double damping;
extern const double viscosity;
extern const double particleSize;
extern const double timeStep;

// Simulation vector constants
extern const std::vector<double> externalAcceleration;
extern const std::vector<double> boxUpperBound;
extern const std::vector<double> boxLowerBound;
} // namespace Constants

#endif // FLUID_CONSTANTS_HPP
