#include "grid.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <unordered_map>

// Constructor and Destructor
Grid::Grid(float ppm, int np) : ppm(ppm), np(np) {
    particleMass = Constants::fluidDensity / pow(ppm, 3);
    smoothingLength = Constants::radiusMultiplier / ppm;

    slSq = pow(smoothingLength, 2);
    slCu = pow(smoothingLength, 3);
    slSixth = pow(smoothingLength, 6);
    slNinth = pow(smoothingLength, 9);

    numberX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) /
              smoothingLength;
    numberY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) /
              smoothingLength;
    numberZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) /
              smoothingLength;
    numberVector = {numberX, numberY, numberZ};

    numBlocks = numberX * numberY * numberZ;

    sizeX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) / numberX;
    sizeY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) / numberY;
    sizeZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) / numberZ;
    sizesVector = {sizeX, sizeY, sizeZ};

    densTransConstant = (315.0 / 64 * M_PI * slNinth) * particleMass;
    accTransConstant1 = (15 / M_PI * slSixth) *
                        ((3 * particleMass * Constants::stiffnessPressure) / 2);
    accTransConstant2 =
        (45 / M_PI * slSixth) * Constants::viscosity * particleMass;

    blocks = std::unordered_map<std::vector<int>, Block, hashing::vHash>();
  update_grid();
}

Grid::~Grid() = default;

// Getters and setters for each variable
std::unordered_map<std::vector<int>, Block, hashing::vHash> Grid::get_blocks() {
  return blocks;
}

float Grid::get_ppm() const { return ppm; }
int Grid::get_np() const { return np; }

double Grid::get_particleMass() const { return particleMass; }
double Grid::get_smoothingLength() const { return smoothingLength; }
double Grid::get_numberX() const { return numberX; }
double Grid::get_numberY() const { return numberY; }
double Grid::get_numberZ() const { return numberZ; }
double Grid::get_numBlocks() const { return numBlocks; }
double Grid::get_sizeX() const { return sizeX; }
double Grid::get_sizeY() const { return sizeY; }
double Grid::get_sizeZ() const { return sizeZ; }
double Grid::get_slSq() const {return slSq;}
double Grid::get_slCu() const {return slCu;}
double Grid::get_slSixth() const { return slSixth; }
double Grid::get_slNinth() const { return slNinth; }
double Grid::get_densTransConstant() const { return densTransConstant; }
double Grid::get_accTransConstant1() const { return accTransConstant1; }
double Grid::get_accTransConstant2() const { return accTransConstant2; }

// block functions
void Grid::add_particle_to_block(const Particle& particle) {
  std::vector<int> key = findBlock(particle);

  auto it = blocks.find(key);

  if (it != blocks.end())
  {
    // Already exists
    auto targetBlock = it->second;
    targetBlock.addParticle(particle);
  }
  else
  {
    Block newBlock(key);
    blocks.insert({key, newBlock});
  }
//  if (get_blocks().count(key) > 0) {
//    // Key is present
//    blocks[key].addParticle(particle);
//
//  } else {
//    // Key is not present
//    Block b(key);
//    blocks.insert(std::make_pair(key, b));
//  }
}

// update simulation parameters
void Grid::update_grid() {
  particleMass = Constants::fluidDensity / pow(ppm, 3);
  smoothingLength = Constants::radiusMultiplier / ppm;

  slSq = pow(smoothingLength, 2);
  slCu = pow(smoothingLength, 3);
  slSixth = pow(smoothingLength, 6);
  slNinth = pow(smoothingLength, 9);

  numberX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) /
            smoothingLength;
  numberY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) /
            smoothingLength;
  numberZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) /
            smoothingLength;
  numberVector = {numberX, numberY, numberZ};

  numBlocks = numberX * numberY * numberZ;

  sizeX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) / numberX;
  sizeY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) / numberY;
  sizeZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) / numberZ;
  sizesVector = {sizeX, sizeY, sizeZ};

  densTransConstant = (315.0 / 64 * M_PI * slNinth) * particleMass;
  accTransConstant1 = (15 / M_PI * slSixth) *
                      ((3 * particleMass * Constants::stiffnessPressure) / 2);
  accTransConstant2 =
      (45 / M_PI * slSixth) * Constants::viscosity * particleMass;
}

// Find the block that a particle belongs in
// ** NEED TO ACCOUNT FOR EDGE CASES OF SURPASSING BOUNDARIES
std::vector<int> Grid::findBlock(Particle part) {
  std::vector<float> position = part.get_position();
    // First need to confirm that each particle is in bounds
    for (int i = 0; i < 3; i++) {
      if (position[i] > Constants::boxUpperBound[i]) {
        position[i] = static_cast<float>(Constants::boxUpperBound[i]);
      }
      else if (position[i] < Constants::boxLowerBound[i]) {
        position[i] = static_cast<float>(Constants::boxLowerBound[i]);
      }
    }
  // Now, need to find the specific block a particle occupies
    // This is done by finding which block index the particle has in all three dimensions
    std::vector<int> blockIndices = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
      blockIndices[i] = static_cast<int>((position[i] - Constants::boxLowerBound[i]) / sizesVector[i]);
    }
  // We must now check that these block coordinates obey their appropriate boundaries
    for (int i = 0; i < 3; i++) {
      if (blockIndices[i] < 0) {
        blockIndices[i] = 0;
      } else if (blockIndices[i] > (numberVector[i] - 1)) {
        blockIndices[i] = static_cast<int>(numberVector[i] - 1);
      }
    }
  return blockIndices;
}

void Grid::findAdjBlocks(Block centerBlock) const {
  std::vector<int> centerIndex = centerBlock.get_index();
  // std::unordered_map<std::vector<int>, Block, hashing::vHash> blockDict;

  for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        for (int k = -1; k <= 1; k++) {
          int newX = centerIndex[0] + i;
          int newY = centerIndex[1] + j;
          int newZ = centerIndex[2] + k;

          // Want to add a check here to see if the potential adjacent blocks actually exist within the grid
          // If it does, we want to add it to the result
          if (newX >= 0 && newX <= (numberX - 1) && newY >= 0 && newY <= (numberY - 1) && newZ >= 0 && newZ <= (numberZ - 1)) {
            std::vector<int> adjBlockIndex = {newX, newY, newZ};
            Block adjBlock(adjBlockIndex);
            {
              centerBlock.addAdjacentBlock(adjBlock);
            }
          }
        }
      }
  }
}
