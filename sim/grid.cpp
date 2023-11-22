#include "grid.hpp"

const float threeonefive = 315.0;
const int sixtyfour = 64;
const int nine = 9;
const int fifteen = 15;
const int fourtyfive = 45;

// Constructor and Destructor
Grid::Grid(float ppm, int np)
    : ppm(ppm), np(np), particleMass(Constants::fluidDensity / pow(ppm, 3)),
      smoothingLength(Constants::radiusMultiplier / ppm) {
  update_grid();
}

Grid::~Grid() = default;

// Getters and setters for each variable
std::unordered_map<std::vector<int>, Block, hashing::vHash>
Grid::get_blocks() const {
  return blocks;
}

float Grid::get_ppm() const { return ppm; }
int Grid::get_np() const { return np; }
int Grid::get_count() const { return count; }
void Grid::set_count(int newCount) { Grid::count = newCount; }

double Grid::get_particleMass() const { return particleMass; }
double Grid::get_smoothingLength() const { return smoothingLength; }
double Grid::get_numberX() const { return numberX; }
double Grid::get_numberY() const { return numberY; }
double Grid::get_numberZ() const { return numberZ; }
double Grid::get_sizeX() const { return sizeX; }
double Grid::get_sizeY() const { return sizeY; }
double Grid::get_sizeZ() const { return sizeZ; }
double Grid::get_slSq() const { return slSq; }
double Grid::get_slCu() const { return slCu; }
double Grid::get_slSixth() const { return slSixth; }
double Grid::get_slNinth() const { return slNinth; }
double Grid::get_densTransConstant() const { return densTransConstant; }
double Grid::get_accTransConstant1() const { return accTransConstant1; }
double Grid::get_accTransConstant2() const { return accTransConstant2; }

// block functions
void Grid::add_particle_to_block(const Particle &particle) {
  std::vector<int> const key = findBlock(particle);

  auto itr = blocks.find(key);

  if (itr != blocks.end()) {
    // Already exists
    auto targetBlock = itr->second;
    targetBlock.addParticle(particle);
  } else {
    Block const newBlock(key);
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
  slSq = pow(smoothingLength, 2);
  slCu = pow(smoothingLength, 3);
  slSixth = pow(smoothingLength, six);
  slNinth = pow(smoothingLength, nine);

  const auto &upperBound = Constants::getBoxUpperBound();
  const auto &lowerBound = Constants::getBoxLowerBound();

  numberX = (upperBound[0] - lowerBound[0]) / smoothingLength;
  numberY = (upperBound[1] - lowerBound[1]) / smoothingLength;
  numberZ = (upperBound[2] - lowerBound[2]) / smoothingLength;
  numberVector = {numberX, numberY, numberZ};

  sizeX = (upperBound[0] - lowerBound[0]) / numberX;
  sizeY = (upperBound[1] - lowerBound[1]) / numberY;
  sizeZ = (upperBound[2] - lowerBound[2]) / numberZ;
  sizesVector = {sizeX, sizeY, sizeZ};
  densTransConstant =
      (threeonefive / sixtyfour * M_PI * slNinth) * particleMass;
  accTransConstant1 = (fifteen / M_PI * slSixth) *
                      ((3 * particleMass * Constants::stiffnessPressure) / 2);
  accTransConstant2 =
      (fourtyfive / M_PI * slSixth) * Constants::viscosity * particleMass;
}

// Find the block that a particle belongs in
// ** NEED TO ACCOUNT FOR EDGE CASES OF SURPASSING BOUNDARIES
std::vector<int> Grid::findBlock(Particle part) {
  std::vector<float> position = part.get_position();
  position = moveParticleInBounds(position);
  // Now, need to find the specific block a particle occupies
  // by finding which block index the particle has in all three dimensions
  std::vector<int> blockIndices = {0, 0, 0};
  for (int i = 0; i < 3; i++) {
    blockIndices[i] = static_cast<int>(
        (position[i] - Constants::getBoxLowerBound()[i]) / sizesVector[i]);
  }
  // We must now check that these block coordinates obey their appropriate
  // boundaries
  for (int i = 0; i < 3; i++) {
    if (blockIndices[i] < 0) {
      blockIndices[i] = 0;
    } else if (blockIndices[i] > (numberVector[i] - 1)) {
      blockIndices[i] = static_cast<int>(numberVector[i] - 1);
    }
  }
  return blockIndices;
}

std::vector<float> Grid::moveParticleInBounds(std::vector<float> position) {
  for (int i = 0; i < 3; i++) {
    if (position[i] > Constants::getBoxUpperBound()[i]) {
      position[i] = static_cast<float>(Constants::getBoxUpperBound()[i]);
    } else if (position[i] < Constants::getBoxLowerBound()[i]) {
      position[i] = static_cast<float>(Constants::getBoxLowerBound()[i]);
    }
  }

  return position;
}

void Grid::findAdjBlocks(Block centerBlock) const {
  std::vector<int> centerIndex = centerBlock.get_index();
  // std::unordered_map<std::vector<int>, Block, hashing::vHash> blockDict;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      for (int k = -1; k <= 1; k++) {
        int const newX = centerIndex[0] + i;
        int const newY = centerIndex[1] + j;
        int const newZ = centerIndex[2] + k;

        // Want to add a check here to see if the potential adjacent blocks
        // actually exist within the grid If it does, we want to add it to the
        // result
        if (newX >= 0 && newX <= (numberX - 1) && newY >= 0 &&
            newY <= (numberY - 1) && newZ >= 0 && newZ <= (numberZ - 1)) {
          std::vector<int> const adjBlockIndex = {newX, newY, newZ};
          Block const adjBlock(adjBlockIndex);
          { centerBlock.addAdjacentBlock(adjBlock); }
        }
      }
    }
  }
}
