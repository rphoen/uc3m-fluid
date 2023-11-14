#ifndef BLOCK_CPP
#define BLOCK_CPP

#include <math.h>
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
  extern double slSq;
  extern double slCu;
  extern double slSixth;
  extern double slNinth;

  extern void update_sim_params();
}; // namespace Constants

// Particle class
class Particle {
public:
  // Member variables
  std::vector<float> position;
  std::vector<float> hv;
  std::vector<float> velocity;
  double density;
  std::vector<double> acceleration;

  // Constructor
  Particle(float px, float py, float pz, float hvx, float hvy, float hvz,
           float vx, float vy, float vz);

  // Destructor
  ~Particle();
};

namespace Box {
  // Calculating the number of blocks in each dimension...
  extern double numberX;
  extern double numberY;
  extern double numberZ;

  extern double numBlocks;

  // Calculating the size of grid blocks in each dimension...
  extern double sizeX;
  extern double sizeY;
  extern double sizeZ;

  // Update variables
  void update_box_params();

  // Determining which block a particle belongs in...
  std::vector<int> findBlock(Particle part);
}; // namespace Box

namespace Formulas {
  // Repositioning particles...
  // ........

  // Increasing density...
  void incDensity(Particle iPart, Particle jPart);

  double findDistance(Particle iPart, Particle jPart);

  void accelerationTransfer(Particle iPart, Particle jPart);

  void particleMotion(Particle iPart);

  void boxCollisions(Particle iPart);

  void boundaryCollisions(Particle iPart);
}; // namespace Formulas

#endif
