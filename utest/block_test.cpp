#include "gtest/gtest.h"
#include "../sim/block.hpp"
#include "../sim/constants.hpp"
#include "../sim/particle.hpp"
#include "../sim/grid.hpp"

// g++ -o utest/block_test utest/block_test.cpp sim/block.cpp -lgtest -lpthread

TEST(BlockTest, ConstructorWithValidBlockIndex) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Check that the block's index is correct
  ASSERT_EQ(block.get_index()[0], 0);
  ASSERT_EQ(block.get_index()[1], 0);
  ASSERT_EQ(block.get_index()[2], 0);
}

TEST(BlockTest, GetEmptyParticleVector) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Check that the block's particles vector is empty
  ASSERT_EQ(block.getParticles().size(), 0);
}

TEST(BlockTest, AddParticle) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  Particle particle(1, {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0});
  block.addParticle(particle);

  // Check that the block's particles vector contains the added particle
  ASSERT_EQ(block.getParticles().size(), 1);
  ASSERT_EQ(block.getParticles()[0].get_id(), 1);
  ASSERT_EQ(block.getParticles()[0].get_position(), (std::vector<float>{1.0, 2.0, 3.0}));
  ASSERT_EQ(block.getParticles()[0].get_hv(), (std::vector<float>{4.0, 5.0, 6.0}));
  ASSERT_EQ(block.getParticles()[0].get_velocity(), (std::vector<float>{7.0, 8.0, 9.0}));
}

TEST(BlockTest, IncreaseDensity) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);
  Grid grid(10.0, 1000);

  // Create a particle and add it to the block
  Particle particle(4, {0.01, 0.01, 0.01}, {0.01, 0.0, 0.0}, {0.02, 0.0, 0.0});
  block.addParticle(particle);

  // Increase the density of the particle
  block.incDensity(particle, grid.get_slSq(),
                   grid.get_slSixth(),
                   grid.get_densTransConstant());

  // Check that the particle's density has increased
  ASSERT_EQ(particle.get_density(), Constants::fluidDensity * 1.25);
}

TEST(BlockTest, FindDistance) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create two particles and add them to the block
  Particle particle1(7, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});
  Particle particle2(8, {1.01, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});
  block.addParticle(particle1);
  block.addParticle(particle2);

  // Find the distance between the particles
  double distance = block.findDistance(particle1, particle2);

  // Check that the distance is calculated correctly
  ASSERT_EQ(distance, 0.01);
}

TEST(BlockTest, AccelerationTransfer) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);
  Grid grid(10.0, 1000);

  // Create two particles and add them to the block
  Particle particle1(9, {0.01, 0.02, 0.0}, {0, -0.01, 0}, {0, 0, 0});
  Particle particle2(10, {0.01, 0.01, 0.0}, {0, 0, 0}, {0, 0, 0});
  block.addParticle(particle1);
  block.addParticle(particle2);

  // Transfer acceleration between the particles
  block.accelerationTransfer(particle1, grid.get_slSq(),
                              grid.get_accTransConstant1(),
                              grid.get_accTransConstant2());

  // Check that the particles' accelerations have been updated
  ASSERT_EQ(particle1.get_ax(), -0.01625);
  ASSERT_EQ(particle1.get_ay(), 0.01625);
  ASSERT_EQ(particle1.get_az(), 0.0);

  ASSERT_EQ(particle2.get_ax(), 0.01625);
  ASSERT_EQ(particle2.get_ay(), -0.01625);
  ASSERT_EQ(particle2.get_az(), 0.0);
}

TEST(BlockTest, UpdateParticleMotion) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  Particle particle(11, {0.01, 0.0, 0.0}, {0.01, 0.0, 0.0}, {0.02, 0.0, 0.0});
  block.addParticle(particle);

  // Update the particle's motion
  block.particleMotion(particle);

  // Check that the particle's position, velocity, and hv have been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.01, 0.0, 0.0}));
  ASSERT_NE(particle.get_velocity(), (std::vector<float>{0.01, 0.0, 0.0}));
  ASSERT_NE(particle.get_hv(), (std::vector<float>{0.02, 0.0, 0.0}));
}

TEST(BlockTest, ProcessParticleBoxCollisions) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  Particle particle(12, {0.063, 0.02, 0.03}, {0.02, 0.0, 0.0}, {0.04, 0.0, 0.0});
  block.addParticle(particle);

  // Process particle box collisions
  block.boxCollisions(particle);

  // Check that the particle's position, velocity, and hv have been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.063, 0.02, 0.03}));
  ASSERT_NE(particle.get_velocity(), (std::vector<float>{0.01, 0.0, 0.0}));
  ASSERT_NE(particle.get_hv(), (std::vector<float>{0.02, 0.0, 0.0}));
}

TEST(BlockTest, ProcessParticleBoundaryCollisions) {
  // Create a block with index [0, 0, 0]
  std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  Particle particle(13, {0.063, 0.02, 0.03}, {0.02, 0.0, 0.0}, {0.04, 0.0, 0.0});
  block.addParticle(particle);

  // Process particle boundary collisions
  block.boundaryCollisions(particle);

  // Check that the particle's position has been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.063, 0.02, 0.03}));
}

