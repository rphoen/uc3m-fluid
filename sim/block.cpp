#include "grid.hpp"
#include "block.hpp"
#include "particle.hpp"
#include <cmath>
#include <utility>

// Constructor
Block::Block(std::vector<int> blockIndex)
{
    index = std::move(blockIndex);
};

std::vector<Particle> Block::getParticles() {
  return particles;
};

void Block::addParticle(const Particle& part) {
  particles.emplace_back(part);
};

// Get index
std::vector<int> Block::getIndex() {
  return index;
}

// Increasing density...
void Block::incDensity(const Particle& part, Grid grid) {
  auto px = part.get_px();
  auto py = part.get_py();
  auto pz = part.get_pz();

  for (auto& bk : adjBlocks)
    {
      auto adjparts = bk.getParticles();
     for (const auto& adjpart : adjparts)
       {
        auto px2 = adjpart.get_px();
        auto py2 = adjpart.get_py();
        auto pz2 = adjpart.get_pz();
        auto xDiffSq = pow((px - px2), 2);
        auto yDiffSq = pow((py - py2), 2);
        auto zDiffSq = pow((pz - pz2), 2);
        auto diffSum = xDiffSq + yDiffSq + zDiffSq;  
        auto slSq = grid.get_slSq();
         
        if (diffSum < slSq)
        {
          double densityChange = pow((slSq - diffSum), 3);
          double newDensity = part.get_density();
          double densTransformation = (newDensity + grid.get_slSixth()) * grid.get_densTransConstant();
          part.set_density(densTransformation);
        }
         
        // else if (vecNorm >= Constants::slSq) {
        //   iPart.density += 0;}
      }
  }
};

// Formula to calculate the distance between two particles
double Block::findDistance(Particle iPart, Particle jPart) {
  float ix = iPart.get_px();
  float iy = iPart.get_py();
  float iz = iPart.get_pz();
  float jx = jPart.get_px();
  float jy = jPart.get_py();
  float jz = jPart.get_pz();

  auto xDiffSq = pow((px - px2), 2);
  auto yDiffSq = pow((py - py2), 2);
  auto zDiffSq = pow((pz - pz2), 2);
  auto diffSum = xDiffSq + yDiffSq + zDiffSq;  

  double distance = sqrt(fmax(vecNorm, pow(10, -12)));
  return distance;
};

// Need to make sure acceleration transfers aren't double counted...
void accelerationTransfer(Particle part) {
  // double distance = findDistance(iPart, jPart);
  auto px1 = part.get_px();
  auto py1 = part.get_py();
  auto pz1 = part.get_pz();

  for (const auto& block : adjBlocks) {
    auto adjParts = block.getParticles();
    for(const auto& adjPart : adjParts) {
      auto px2 = adjPart.get_px();
      auto py2 = adjPart.get_py();
      auto pz2 = adjPart.get_pz();

      // TODO: Need to check if distance is short enough first
      double distance = findDistance(px1, py1, pz1, px2, py2, pz2);

      auto accChange = ((((px1 - px2) * grid.get_accTransConstant1() * (pow((grid.smoothingLength - distance), 2)) / distance) * (part.get_density() + adjPart.get_density() - (2 * Constants::fluidDensity)) * grid.get_accTransConstant2()) / (part.get_density() * adjPart.get_density()));

      // TODO: This is wrong. Need to update the accelerations.
      part.set_acceleration(accChange);
      adjPart.set_acceleration(accChange);
      
    }
  }
  for (int i = 0; i < 3; i++) {
    double accChange =
        // Either too little precision, or a computational error
        // Rewrite the code
        ((((iPart.position[i] - jPart.position[i]) *
           Constants::accTransConstant1 *
           (pow((Constants::smoothingLength - distance), 2)) / distance) *
          (iPart.density + jPart.density - (2 * Constants::fluidDensity)) *
          Constants::accTransConstant2) /
         (iPart.density * jPart.density));

    // Extra variable inside particles, a boolean for example
      // to keep track of whether or not a particle has been updated
    iPart.acceleration[i] += accChange;
    jPart.acceleration[i] -= accChange;
  }
};

void particleMotion() {
  for (int i = 0; i < 3; i++) {
    iPart.position[i] =
        iPart.position[i] + iPart.hv[i] * Constants::timeStep + iPart.acceleration[i] * pow(Constants::timeStep, 2);
    iPart.velocity[i] = iPart.hv[i] + ((iPart.acceleration[i] * Constants::timeStep) / 2);
    iPart.hv[i] = iPart.hv[i] + iPart.acceleration[i] * Constants::timeStep;
  }
};

void boxCollisions(Particle iPart) {
  // Checking x-boundary collisions...
  float newX = iPart.position[0] + iPart.hv[0] * Constants::timeStep;

  // Check for both bounds...
  double changeXLower =
      Constants::particleSize - (newX - Constants::boxLowerBound[0]);
  double changeXUpper =
      Constants::particleSize - (newX - Constants::boxUpperBound[0]);

  if (changeXLower > pow(10, -10)) {
    iPart.acceleration[0] += Constants::stiffnessCollisions * changeXLower -
                             Constants::damping * iPart.velocity[0];
  } else if (changeXUpper > pow(10, -10)) {
    iPart.acceleration[0] -= Constants::stiffnessCollisions * changeXLower +
                             Constants::damping * iPart.velocity[0];
  };
  // Checking y-boundary collisions...
  float newY = iPart.position[1] + iPart.hv[1] * Constants::timeStep;

  // Check for both bounds...
  double changeYLower =
      Constants::particleSize - (newY - Constants::boxLowerBound[1]);
  double changeYUpper =
      Constants::particleSize - (newY - Constants::boxUpperBound[1]);

  if (changeYLower > pow(10, -10)) {
    iPart.acceleration[1] += Constants::stiffnessCollisions * changeYLower -
                             Constants::damping * iPart.velocity[1];
  } else if (changeYUpper > pow(10, -10)) {
    iPart.acceleration[1] -= Constants::stiffnessCollisions * changeYLower +
                             Constants::damping * iPart.velocity[1];
  };

  // Checking z-boundary collisions...
  float newZ = iPart.position[2] + iPart.hv[2] * Constants::timeStep;

  // Check for both bounds...
  double changeZLower =
      Constants::particleSize - (newZ - Constants::boxLowerBound[2]);
  double changeZUpper =
      Constants::particleSize - (newZ - Constants::boxUpperBound[2]);

  if (changeZLower > pow(10, -10)) {
    iPart.acceleration[2] += Constants::stiffnessCollisions * changeZLower -
                             Constants::damping * iPart.velocity[2];
  } else if (changeZUpper > pow(10, -10)) {
    iPart.acceleration[2] -= Constants::stiffnessCollisions * changeZLower +
                             Constants::damping * iPart.velocity[2];
  };
};

void boundaryCollisions(Particle iPart) {
  // X-boundary...
  float dXLower = iPart.position[0] - Constants::boxLowerBound[0];
  float dXUpper = Constants::boxUpperBound[0] - iPart.position[0];

  if (dXLower < 0) {
    iPart.position[0] = Constants::boxLowerBound[0] - dXLower;
    iPart.velocity[0] = -1 * iPart.velocity[0];
    iPart.hv[0] = -1 * iPart.hv[0];
  } else if (dXUpper < 0) {
    iPart.position[0] = Constants::boxUpperBound[0] + dXUpper;
    iPart.velocity[0] = -1 * iPart.velocity[0];
    iPart.hv[0] = -1 * iPart.hv[0];
  }

  // Y-boundary...
  float dYLower = iPart.position[1] - Constants::boxLowerBound[1];
  float dYUpper = Constants::boxUpperBound[1] - iPart.position[1];

  if (dYLower < 0) {
    iPart.position[1] = Constants::boxLowerBound[1] - dYLower;
    iPart.velocity[1] = -1 * iPart.velocity[1];
    iPart.hv[1] = -1 * iPart.hv[1];
  } else if (dYUpper < 0) {
    iPart.position[1] = Constants::boxUpperBound[1] + dYUpper;
    iPart.velocity[1] = -1 * iPart.velocity[1];
    iPart.hv[1] = -1 * iPart.hv[1];
  }

  // Z-boundary...
  float dZLower = iPart.position[2] - Constants::boxLowerBound[2];
  float dZUpper = Constants::boxUpperBound[2] - iPart.position[2];

  if (dZLower < 0) {
    iPart.position[2] = Constants::boxLowerBound[2] - dZLower;
    iPart.velocity[2] = -1 * iPart.velocity[2];
    iPart.hv[2] = -1 * iPart.hv[2];
  } else if (dZUpper < 0) {
    iPart.position[2] = Constants::boxUpperBound[2] + dZUpper;
    iPart.velocity[2] = -1 * iPart.velocity[2];
    iPart.hv[2] = -1 * iPart.hv[2];
  }
};

// Block destructor implementation
Block::~Block() = default;