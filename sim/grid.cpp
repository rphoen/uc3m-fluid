#include "grid.hpp"
#include "block.hpp"
#include "constants.hpp"

// Constructor and Destructor
Grid::Grid(float ppm, int np) : ppm(ppm), np(np) { update_grid(); }
Grid::~Grid() {
  // Destructor code, if needed
}

// Getters and setters for each variable
std::unordered_map<std::vector<int>, Block> Grid::get_blocks() {
  return blocks;
}

float Grid::get_ppm() { return ppm; }
int Grid::get_np() { return np; }

double Grid::get_particleMass() { return particleMass; }
double Grid::get_smoothingLength() { return smoothingLength; }
double Grid::get_numberX() { return numberX; }
double Grid::get_numberY() { return numberY; }
double Grid::get_numberZ() { return numberZ; }
double Grid::get_numBlocks() { return numBlocks; }
double Grid::get_sizeX() { return sizeX; }
double Grid::get_sizeY() { return sizeY; }
double Grid::get_sizeZ() { return sizeZ; }
double Grid::get_slSq() { return slSq; }
double Grid::get_slCu() { return slCu; }
double Grid::get_slSixth() { return slSixth; }
double Grid::get_slNinth() { return slNinth; }
double Grid::get_densTransConstant() { return densTransConstant; }
double Grid::get_accTransConstant1() { return accTransConstant1; }
double Grid::get_accTransConstant2() { return accTransConstant2; }

// block functions
void Grid::add_particle_to_block(Particle p) {
  std::vector<int> key = findBlock(p.get_px(), p.get_py(), p.get_pz());

  if (get_blocks().count(key) > 0) {
    // Key is present
    blocks[key].addParticle(p);
  } else {
    // Key is not present
    Block b;
    blocks.insert(std::make_pair(key, b));
  }
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

  numBlocks = numberX * numberY * numberZ;

  sizeX = (Constants::boxUpperBound[0] - Constants::boxLowerBound[0]) / numberX;
  sizeY = (Constants::boxUpperBound[1] - Constants::boxLowerBound[1]) / numberY;
  sizeZ = (Constants::boxUpperBound[2] - Constants::boxLowerBound[2]) / numberZ;

  densTransConstant = (315.0 / 64 * M_PI * slNinth) * particleMass;
  accTransConstant1 = (15 / M_PI * slSixth) *
                      ((3 * particleMass * Constants::stiffnessPressure) / 2);
  accTransConstant2 =
      (45 / M_PI * slSixth) * Constants::viscosity * particleMass;
}

// Find the block that a paricle belongs in
// ** NEED TO ACCOUNT FOR EDGE CASES OF SURPASSING BOUNDARIES
std::vector<int> Grid::findBlock(float px, float py, float pz) {
  // Need to check to see if these are within the bounds
  //   // x-coordinate...
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
  int bK = static_cast<int>((pz - Constants::boxLowerBound[2]) / sizeY);

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
};
