#ifndef BLOCK_CPP
#define BLOCK_CPP

#include <vector>

namespace Constants {
  // Information from initial file
  extern float ppm;
  extern int np;

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

  // Simulation parameters
  extern double particleMass;
  extern double smoothingLength;
};

class Particle {
  public:
    float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
};

#endif
