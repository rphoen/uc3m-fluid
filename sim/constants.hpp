#ifndef FLUID_CONSTANTS_HPP
#define FLUID_CONSTANTS_HPP

namespace Constants {
  // Simulation scalar constants
  extern double radiusMultiplier;
  extern double fluidDensity;
  extern double stiffnessPressure;
  extern double stiffnessCollisions;
  extern double damping;
  extern double viscosity;
  extern double particleSize;
  extern double timeStep;

  // Simulation vector constants
  extern std::vector<double> externalAcceleration;
  extern std::vector<double> boxUpperBound;
  extern std::vector<double> boxLowerBound;
} // namespace Constants

#endif  // FLUID_CONSTANTS_HPP
