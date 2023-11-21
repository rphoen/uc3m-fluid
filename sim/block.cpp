#include "block.hpp"
#include "constants.hpp"
#include <cmath>
#include <utility>
#include <unordered_map>

// Constructor
Block::Block(std::vector<int> blockIndex, Grid* simGrid)
{
    index = std::move(blockIndex);

    //particles = ...; // ???

    auto numX = simGrid->get_numberX();
    auto numY = simGrid->get_numberY();
    auto numZ = simGrid->get_numberZ();

    adjBlocks = {};
    findAdjBlocks(numX, numY, numZ, simGrid);

}

// Return a vector of all particles that belong to a specific block
std::vector<Particle> Block::getParticles() {
  return particles;
}

// Add a particle to the vector of all particles belonging to a specific block
void Block::addParticle(const Particle& part) {
  particles.emplace_back(part);
}

// Increasing density between a given particle and every particle in the adjacent blocks
void Block::incDensity(Particle& part, const Grid& grid) {
  auto px = part.get_px();
  auto py = part.get_py();
  auto pz = part.get_pz();

  for (auto& bk : adjBlocks)
    {
      auto adjParts = bk.getParticles();
     for (const auto& adjPart : adjParts)
       {
        auto px2 = adjPart.get_px();
        auto py2 = adjPart.get_py();
        auto pz2 = adjPart.get_pz();
        auto xDiffSq = pow((px - px2), 2);
        auto yDiffSq = pow((py - py2), 2);
        auto zDiffSq = pow((pz - pz2), 2);
        auto diffSum = xDiffSq + yDiffSq + zDiffSq;  
        auto slSq = grid.get_slSq();
         
        if (diffSum < slSq) {
          double densityChange = pow((slSq - diffSum), 3);
          double newDensity    = part.get_density() + densityChange;
          double densTransformation =
              (newDensity + grid.get_slSixth()) * grid.get_densTransConstant();
          part.set_density(densTransformation);
        }
        // else if (vecNorm >= Constants::slSq) {
        //   iPart.density += 0;}
      }
  }
}

// Formula to calculate the distance between two given particles
double Block::findDistance(const Particle& iPart, const Particle& jPart) {
  float ix = iPart.get_px();
  float iy = iPart.get_py();
  float iz = iPart.get_pz();
  float jx = jPart.get_px();
  float jy = jPart.get_py();
  float jz = jPart.get_pz();

  auto xDiffSq = pow((ix - jx), 2);
  auto yDiffSq = pow((iy - jy), 2);
  auto zDiffSq = pow((iz - jz), 2);
  auto diffSum = xDiffSq + yDiffSq + zDiffSq;  

  double distance = sqrt(fmax(diffSum, pow(10, -12)));
  return distance;
}

// Transfer accelerations between a given particle and every particle in the adjacent blocks
// NEED TO MAKE SHORTER
void Block::accelerationTransfer(Particle part, const Grid& grid) {
  // double distance = findDistance(iPart, jPart);
  auto px1 = part.get_px();
  auto py1 = part.get_py();
  auto pz1 = part.get_pz();

  for (auto& block : adjBlocks) {
      auto adjParts = block.getParticles();
      for (auto & adjPart : adjParts) {
        auto px2 = adjPart.get_px();
        auto py2 = adjPart.get_py();
        auto pz2 = adjPart.get_pz();

        // Need to check if distance is short enough first
        auto xDiffSq = pow((px1 - px2), 2);
        auto yDiffSq = pow((py1 - py2), 2);
        auto zDiffSq = pow((pz1 - pz2), 2);
        auto diffSum = xDiffSq + yDiffSq + zDiffSq;
        auto slSq    = grid.get_slSq();

        if (diffSum < slSq)  // Update the acceleration
        {
          double distance = findDistance(part, adjPart);
          // Change this to helper function eventually
          auto xAccChange =
              (((px1 - px2) * grid.get_accTransConstant1() *
                ((pow((grid.get_slSq() - distance), 2)) / distance) *
                (part.get_density() + adjPart.get_density() - (2 * Constants::fluidDensity)) *
                grid.get_accTransConstant2()) /
               (part.get_density() * adjPart.get_density()));

          auto yAccChange =
              (((py1 - py2) * grid.get_accTransConstant1() *
                ((pow((grid.get_slSq() - distance), 2)) / distance) *
                (part.get_density() + adjPart.get_density() - (2 * Constants::fluidDensity)) *
                grid.get_accTransConstant2()) /
               (part.get_density() * adjPart.get_density()));

          auto zAccChange =
              (((pz1 - pz2) * grid.get_accTransConstant1() *
                ((pow((grid.get_slSq() - distance), 2)) / distance) *
                (part.get_density() + adjPart.get_density() - (2 * Constants::fluidDensity)) *
                grid.get_accTransConstant2()) /
               (part.get_density() * adjPart.get_density()));

          if (!part.hasAccelerated()) {
            std::vector<double> partNew = {(part.get_ax() + xAccChange), (part.get_ay() + yAccChange), (part.get_az() + zAccChange)};
            std::vector<double> adjNew = {(adjPart.get_ax() - xAccChange), (adjPart.get_ay() - yAccChange), (adjPart.get_az() - zAccChange)};
            part.set_acceleration(partNew);
            part.updateAccBool();
            adjPart.set_acceleration(adjNew);
            adjPart.updateAccBool();
          }
        }
      }
  }
}

// Update a particle (i.e., its position, hv, and velocity
void Block::particleMotion(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> hv = part.get_hv();
  std::vector<float> velocity = part.get_velocity();
  std::vector<double> acceleration = part.get_acceleration();

  for (int i = 0; i < 3; i++) {
    position[i] = static_cast<float>(position[i] + hv[i] * Constants::timeStep + acceleration[i] * pow(Constants::timeStep, 2));
    velocity[i] = static_cast<float>(hv[i] + ((acceleration[i] * Constants::timeStep) / 2));
    hv[i] = static_cast<float>(hv[i] + acceleration[i] * Constants::timeStep);
  }

  part.set_position(position);
  part.set_velocity(velocity);
  part.set_hv(hv);
}

// Process the box collisions of one particle
void Block::boxCollisions(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> hv = part.get_hv();
  std::vector<float> velocity = part.get_velocity();
  std::vector<double> currentAcc = part.get_acceleration();
  std::vector<double> newAcc = part.get_acceleration();

  for (int i = 0; i < 3; i++)
  {
    auto newCoord = static_cast<float>(position[i] + hv[i] * Constants::timeStep);
    double changeLower = Constants::particleSize - (newCoord - Constants::boxLowerBound[i]);
    double changeUpper = Constants::particleSize - (Constants::boxUpperBound[i] - newCoord);
    auto check = pow(10, -10);

    if (changeLower > check)
    {
        newAcc[i] = currentAcc[i] + Constants::stiffnessCollisions * changeLower - Constants::damping * velocity[i];

    }
    else if (changeUpper > check)
    {
        newAcc[i] = currentAcc[i] - Constants::stiffnessCollisions * changeLower - Constants::damping * velocity[i];
    }
  }
  part.set_acceleration(newAcc);
}

// Process the boundary collisions of one particle
void Block::boundaryCollisions(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> velocity = part.get_velocity();
  std::vector<float> hv = part.get_hv();


  for (int i = 0; i < 3; i++)
  {
    auto dLower = position[i] - Constants::boxLowerBound[i];
    auto dUpper = Constants::boxUpperBound[i] - position[i];

    if (dLower < 0)
    {
        position[i] = static_cast<float>(Constants::boxLowerBound[i] - dLower);
        velocity[i] = -1 * velocity[i];
        hv[i] = -1 * hv[i];
    }
    else if (dUpper < 0)
    {
        position[i] = static_cast<float>(Constants::boxUpperBound[i] + dUpper);
        velocity[i] = -1 * velocity[i];
        hv[i] = -1 * hv[i];
    }
  }
  part.set_position(position);
  part.set_velocity(velocity);
  part.set_hv(hv);
}

// Need a function to find all adjacent blocks
// Essentially, need to find any blocks with any combination of x+-1, y+-1, and z+-1
// Can be up to 26...
  // BUT!!! CAN BE LESS!!!

// <x+1, y, z>  <x+1, y+1, z>  <x+1, y-1, z>  <x+1, y+1, z+1>  <x+1, y+1, z-1>  <x+1, y-1, z+1>  <x+1, y-1, z-1>  <x+1, y, z+1>  <x+1, y, z-1>
// <x-1, y, z>  <x-1, y+1, z>  <x-1, y-1, z>  <x-1, y+1, z+1>  <x-1, y+1, z-1>  <x-1, y-1, z+1>  <x-1, y-1, z-1>  <x-1, y, z+1>  <x-1, y, z-1>
// <x, y, z>    <x,   y+1, z>  <x,   y-1, z>  <x,   y+1, z+1>  <x,   y+1, z-1>  <x,   y-1, z+1>  <x,   y-1, z-1>  <x,   y, z+1>  <x,   y, z-1>

void Block::findAdjBlocks(auto numX, auto numY, auto numZ, Grid * simGrid) {
  std::vector<int> centerBlock = index;
  std::unordered_map<std::vector<int>, Block, hashing::vHash> blockDict = simGrid->get_blocks();
  std::vector<Block> adjBlocksVec;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
        for (int k = -1; k <= 1; k++) {
          int newX = centerBlock[0] + i;
          int newY = centerBlock[1] + j;
          int newZ = centerBlock[2] + k;

          // Want to add a check here to see if the potential adjacent blocks actually exist within the grid
          // If it does, we want to add it to the result
          if (newX >= 0 && newX <= (numX - 1) && newY >= 0 && newY <= (numY - 1) && newZ >= 0 && newZ <= (numZ - 1)) {
            std::vector<int> adjBlockIndex = {newX, newY, newZ};
            Block adjBlock(adjBlockIndex, (Grid *) simGrid);
            {
              adjBlocksVec.emplace_back(adjBlock);
            }
          }
        }
    }
  }
  adjBlocks = adjBlocksVec;
}

// Block destructor implementation
Block::~Block() = default;