#include "gtest/gtest.h"
#include "../sim/grid.hpp"

TEST(GridConstructorTest, ConstructorWithCorrectValues) {
  // Create a grid with 10 particles per million (ppm) and 1000 particles
  Grid const grid(10.0, 1000);
  const auto &upperBound = Constants::getBoxUpperBound();
  const auto &lowerBound = Constants::getBoxLowerBound();

  // Check that the grid's variables are initialized correctly
  ASSERT_EQ(grid.get_ppm(), 10.0);
  ASSERT_EQ(grid.get_np(), 1000);
  ASSERT_EQ(grid.get_particleMass(), Constants::fluidDensity / pow(10.0, 3));
  ASSERT_EQ(grid.get_smoothingLength(), Constants::radiusMultiplier / 10.0);
  ASSERT_EQ(grid.get_numberX(), (upperBound[0] - lowerBound[0]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_numberY(), (upperBound[1] - lowerBound[1]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_numberZ(), (upperBound[2] - lowerBound[2]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_sizeX(), (upperBound[0] - lowerBound[0]) / grid.get_numberX());
  ASSERT_EQ(grid.get_sizeY(), (upperBound[1] - lowerBound[1]) / grid.get_numberY());
  ASSERT_EQ(grid.get_sizeZ(), (upperBound[2] - lowerBound[2]) / grid.get_numberZ());
  ASSERT_EQ(grid.get_slSq(), pow(grid.get_smoothingLength(), 2));
  ASSERT_EQ(grid.get_slCu(), pow(grid.get_smoothingLength(), 3));
  ASSERT_EQ(grid.get_slSixth(), pow(grid.get_smoothingLength(), 6));
  ASSERT_EQ(grid.get_slNinth(), pow(grid.get_smoothingLength(), 9));
  ASSERT_EQ(grid.get_densTransConstant(), (315.0 / 64 * M_PI * grid.get_slNinth()) * grid.get_particleMass());
  ASSERT_EQ(grid.get_accTransConstant1(), (15 / M_PI * grid.get_slSixth()) * ((3 * grid.get_particleMass() * Constants::stiffnessPressure) / 2));
  ASSERT_EQ(grid.get_accTransConstant2(), (45 / M_PI * grid.get_slSixth()) * Constants::viscosity * grid.get_particleMass());
}

TEST(GridAddParticleToBlockTest, AddParticleToExistingBlock) {
  // Create a grid with 10 particles per million (ppm) and 1000 particles
  const int ppm = 10;
  const int npnp = 1000;
  Grid grid(ppm, npnp);

  // Create a particle and add it to the grid
  Particle const particle(1, {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0});
  grid.add_particle_to_block(particle);

  // Check that the particle is in the grid's blocks
  std::vector<int> const blockIndices = grid.findBlock(particle);
  ASSERT_EQ(grid.get_blocks().count(blockIndices), 1);
//   ASSERT_EQ(grid.get_blocks()[blockIndices].get_particles().size(), 1);
}

TEST(GridAddParticleToBlockTest, AddParticleToNewBlock) {
  // Create a grid with 10 particles per million (ppm) and 1000 particles
const int ppm = 10;
const int npnp = 1000;
  Grid grid(ppm, npnp);

  // Create a particle and add it to the grid
  Particle const particle(2, {11.0, 12.0, 13.0}, {14.0, 15.0, 16.0}, {17.0, 18.0, 19.0});
  grid.add_particle_to_block(particle);

  // Check that the particle is in the grid's blocks
  std::vector<int> const blockIndices = grid.findBlock(particle);
  ASSERT_EQ(grid.get_blocks().count(blockIndices), 1);
//   ASSERT_EQ(grid.get_blocks()[blockIndices].get_particles().size(), 1);
}

TEST(GridUpdateGridTest, UpdateGridWithNewParameters) {
  // Create a grid with 10 particles per million (ppm) and 1000 particles
  const int ppm = 10;
  const int npnp = 1000;
  Grid grid(ppm, npnp);
  const auto &upperBound = Constants::getBoxUpperBound();
  const auto &lowerBound = Constants::getBoxLowerBound();

  // Update the grid's parameters
  grid.update_grid();

  // Check that the grid's variables are updated correctly
  ASSERT_EQ(grid.get_ppm(), 10.0);
  ASSERT_EQ(grid.get_np(), 1000);
  ASSERT_EQ(grid.get_particleMass(), Constants::fluidDensity / pow(10.0, 3));
  ASSERT_EQ(grid.get_smoothingLength(), Constants::radiusMultiplier / 10.0);
  ASSERT_EQ(grid.get_numberX(), (upperBound[0] - lowerBound[0]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_numberY(), (upperBound[1] - lowerBound[1]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_numberZ(), (upperBound[2] - lowerBound[2]) / (Constants::radiusMultiplier / 10.0));
  ASSERT_EQ(grid.get_sizeX(), (upperBound[0] - lowerBound[0]) / grid.get_numberX());
  ASSERT_EQ(grid.get_sizeY(), (upperBound[1] - lowerBound[1]) / grid.get_numberY());
  ASSERT_EQ(grid.get_sizeZ(), (upperBound[2] - lowerBound[2]) / grid.get_numberZ());
  ASSERT_EQ(grid.get_slSq(), pow(grid.get_smoothingLength(), 2));
  ASSERT_EQ(grid.get_slCu(), pow(grid.get_smoothingLength(), 3));
  ASSERT_EQ(grid.get_slSixth(), pow(grid.get_smoothingLength(), 6));
  ASSERT_EQ(grid.get_slNinth(), pow(grid.get_smoothingLength(), 9));
  ASSERT_EQ(grid.get_densTransConstant(), (315.0 / 64 * M_PI * grid.get_slNinth()) * grid.get_particleMass());
  ASSERT_EQ(grid.get_accTransConstant1(), (15 / M_PI * grid.get_slSixth()) * ((3 * grid.get_particleMass() * Constants::stiffnessPressure) / 2));
  ASSERT_EQ(grid.get_accTransConstant2(), (45 / M_PI * grid.get_slSixth()) * Constants::viscosity * grid.get_particleMass());
}

TEST(GridFindBlockTest, FindBlockWithValidParticlePosition) {
  // Create a grid with 10 particles per million (ppm) and 1000 particles
  const int ppm = 10;
  const int npnp = 1000;
  Grid grid(ppm, npnp);

  // Create a particle and find its block
  Particle const particle(3, {0.1, 0.2, 0.3}, {0.1, 0.1, 0.1}, {0.2, 0.2, 0.2});
  std::vector<int> blockIndices = grid.findBlock(particle);

  // Check that the block indices are correct
  ASSERT_EQ(blockIndices[0], 0);
  ASSERT_EQ(blockIndices[1], 0);
  ASSERT_EQ(blockIndices[2], 0);
}
