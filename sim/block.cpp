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
double slSq = pow(smoothingLength, 2);
double slCu = pow(smoothingLength, 3);
double slSixth = pow(smoothingLength, 6);
double slNinth = pow(smoothingLength, 9);

double densTransConstant = (315 / 64 * M_PI * slNinth) * particleMass;
double accTransConstant1 =
    (15 / M_PI * slSixth) * ((3 * particleMass * stiffnessPressure) / 2);
double accTransConstant2 = (45 / M_PI * slSixth) * viscosity * particleMass;
}; // namespace Constants

// update simulation parameters
void update_sim_params() {
  Constants::particleMass = Constants::fluidDensity / pow(Constants::ppm, 3);
  Constants::smoothingLength = Constants::radiusMultiplier / Constants::ppm;
  Constants::slSq = pow(Constants::smoothingLength, 2);
  Constants::slCu = pow(Constants::smoothingLength, 3);
  Constants::slSixth = pow(Constants::smoothingLength, 6);
  Constants::slNinth = pow(Constants::smoothingLength, 9);
}

// Constructor
Particle::Particle(float px, float py, float pz, float hvx, float hvy,
                   float hvz, float vx, float vy, float vz) {
  // Initialize position, half-velocity, and velocity vectors
  position = {px, py, pz};
  hv = {hvx, hvy, hvz};
  velocity = {vx, vy, vz};

  // Initialize other member variables
  density = 0.0;
  acceleration = Constants::externalAcceleration;
}

// Particle destructor implementation
Particle::~Particle() {
  // Destructor code, if needed
}

namespace Box {
// Calculating the number of blocks in each dimension...
double numberX;
double numberY;
double numberZ;

double numBlocks;

// Calculating the size of grid blocks in each dimension...
double sizeX;
double sizeY;
double sizeZ;

void update_box_params() {
  Box::numberX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) /
                 Constants::smoothingLength;
  Box::numberY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) /
                 Constants::smoothingLength;
  Box::numberZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) /
                 Constants::smoothingLength;
  Box::numBlocks = Box::numberX * Box::numberY * Box::numberZ;
  Box::sizeX =
      (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) / numberX;
  Box::sizeY =
      (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) / numberY;
  Box::sizeZ =
      (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) / numberZ;
}

// Determining which block a particle belongs in...
// ** NEED TO ACCOUNT FOR EDGE CASES OF SURPASSING BOUNDARIES
std::vector<int> findBlock(Particle part) {
  // First, get the coordinates of the particle...
  double px = part.position[0];
  double py = part.position[1];
  double pz = part.position[2];

  // Now, need to check to see if these are within the bounds
  // x-coordinate...
  if (px > Constants::boxUpperBound[0]) {
    px = static_cast<float>(Constants::boxUpperBound[0]);
  } else if (px < Constants::boxLowerBound[0]) {
    px = static_cast<float>(Constants::boxLowerBound[0]);
  }

  // y-coordinate...
  if (py > Constants::boxUpperBound[1]) {
    py = static_cast<float>(Constants::boxUpperBound[1]);
  } else if (py < Constants::boxLowerBound[1]) {
    py = static_cast<float>(Constants::boxLowerBound[1]);
  }

  // z-coordinate...
  if (pz > Constants::boxUpperBound[2]) {
    pz = static_cast<float>(Constants::boxUpperBound[2]);
  } else if (pz < Constants::boxLowerBound[2]) {
    pz = static_cast<float>(Constants::boxLowerBound[2]);
  }

  // Now, px, py, and pz should all be within the bounds of the grid
  // So, we can now find the specific block a particle occupies
  // This is done by finding which block index the particle has in all three
  // dimensions
  int bI = static_cast<int>((px - Constants::boxLowerBound[0]) / sizeX);
  int bJ = static_cast<int>((py - Constants::boxLowerBound[1]) / sizeY);
  int bK = static_cast<int>((pz - Constants::boxLowerBound[2]) / sizeZ);

  // We must now check that these block coordinates obey their own boundaries
  // (or otherwise are fixed) x-coordinate...
  if (bI < 0) {
    bI = 0;
  } else if (bI > (numberX - 1)) {
    bI = static_cast<int>(numberX - 1);
  }

  // y-coordinate...
  if (bJ < 0) {
    bJ = 0;
  } else if (bJ > (numberY - 1)) {
    bJ = static_cast<int>(numberY - 1);
  }

  // z-coordinate...
  if (bK < 0) {
    bK = 0;
  } else if (bJ > (numberZ - 1)) {
    bK = static_cast<int>(numberZ - 1);
  }

  // These can then be stored in a vector to represent the specific block a
  // particle is in
  std::vector<int> partBlock = {bI, bJ, bK};
  return partBlock;
}
}; // namespace Box

namespace Formulas {

// Increasing density...
void incDensity(Particle iPart, Particle jPart) {
  float ix = iPart.position[0];
  float iy = iPart.position[1];
  float iz = iPart.position[2];
  float jx = jPart.position[0];
  float jy = jPart.position[1];
  float jz = jPart.position[2];

  // Avoid nesting, possible future hotspot
  // Sqrt and ^2 cancel
  double vecNorm =
      pow(sqrt(pow((ix - jx), 2) + pow((iy - jy), 2) + pow((iz - jz), 2)), 2);
  if (vecNorm < Constants::slSq) {
    double densityChange = pow((Constants::slSq - vecNorm), 3);
    iPart.density += densityChange;
    iPart.density =
        (iPart.density + Constants::slSixth) * Constants::densTransConstant;
  } else if (vecNorm >= Constants::slSq) {
    iPart.density += 0;
  }
};

double findDistance(Particle iPart, Particle jPart) {
  float ix = iPart.position[0];
  float iy = iPart.position[1];
  float iz = iPart.position[2];
  float jx = jPart.position[0];
  float jy = jPart.position[1];
  float jz = jPart.position[2];

  double vecNorm =
      pow(sqrt(pow((ix - jx), 2) + pow((iy - jy), 2) + pow((iz - jz), 2)), 2);

  double distance = sqrt(fmax(vecNorm, pow(10, -12)));
  return distance;
};

// Need to make sure acceleration transfers aren't double counted...
// void accelerationTransfer(Particle iPart, Particle jPart)
// {
// double distance = findDistance(iPart, jPart);
// double accelerationChange  = (((...) * Constants::accTransConstant1 *
// (pow((Constants::smoothingLength - distance), 2) / distance) * (...)) +
// ((...) * Constants::accTransConstant2)) / (...); iPart.acceleration +=
// accelerationChange; jPart.acceleration -= accelerationChange;
//     };
//
// void processBoxCollisions(Particle iPart)
// {
// float newX = iPart.px + (iPart.hvx * 1);
// double changeX = 0.0;
//
// if (...)
// {changeX = Constants::particleSize - (newX - Constants::boxLowerBound[0]);}
// else if (...)
// {changeX = Constants::particleSize - (Constants::boxUpperBound[0] - newX);}
//
//
// if (changeX > pow(10, -10))
// {
//     iPart.acceleration +=
// }};
// };

}; // namespace Formulas
