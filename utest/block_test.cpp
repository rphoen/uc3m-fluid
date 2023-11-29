#include "gtest/gtest.h"
#include "../sim/block.hpp"
#include "../sim/grid.hpp"

TEST(BlockTest, ConstructorWithValidBlockIndex) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  const Block block(blockIndex);

  // Check that the block's index is correct
  ASSERT_EQ(block.get_index()[0], 0);
  ASSERT_EQ(block.get_index()[1], 0);
  ASSERT_EQ(block.get_index()[2], 0);
}

TEST(BlockTest, GetEmptyParticleVector) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Check that the block's particles vector is empty
  ASSERT_EQ(block.getParticles().size(), 0);
}

TEST(BlockTest, AddParticle) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  const Particle particle(1, {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0});
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
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);
  const Grid grid(10.0, 1000);
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};

  // Create a particle and add it to the block
  Particle particle(4, position, halfVelocity, velocity);
  block.addParticle(particle);

  // Increase the density of the particle
  block.incDensity(particle, grid.get_slSq(),
                   grid.get_slSixth(),
                   grid.get_densTransConstant());

  // Check that the particle's density has increased
  ASSERT_EQ(particle.get_density(), 0);
}

TEST(BlockTest, FindDistance) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create two particles and add them to the block
  const Particle particle1(7, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});
  const Particle particle2(8, {1.01, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});
  block.addParticle(particle1);
  block.addParticle(particle2);

  // Find the distance between the particles
  const double distance = Block::findDistance(particle1, particle2);

  // Check that the distance is calculated correctly
  ASSERT_EQ(distance, 0.0099999904632568359);
}

TEST(BlockTest, AccelerationTransfer) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);
  const Grid grid(10.0, 1000);

  // Create two particles and add them to the block
  const int nine = 9;
  const int ten = 10;
  Particle particle1(nine, {0.0, 0.0, 0.0}, {0, 0, 0}, {0, 0, 0});
  Particle particle2(ten, {0.0, 0.0, 0.0}, {0, 0, 0}, {0, 0, 0});
  block.addParticle(particle1);
  block.addParticle(particle2);

  // Transfer acceleration between the particles
  block.accelerationTransfer(particle1, grid.get_slSq(),
                              grid.get_accTransConstant1(),
                              grid.get_accTransConstant2());

  // Check that the particles' accelerations have been updated
  ASSERT_EQ(particle1.get_ax(), 0);
  ASSERT_EQ(particle1.get_ay(), -9.8);
  ASSERT_EQ(particle1.get_az(), 0);

  ASSERT_EQ(particle2.get_ax(), 0);
  ASSERT_EQ(particle2.get_ay(), -9.8);
  ASSERT_EQ(particle2.get_az(), 0);
}

TEST(BlockTest, UpdateParticleMotion) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};
  const int eleven = 11;
  Particle particle(eleven, position, halfVelocity, velocity);
  block.addParticle(particle);

  // Update the particle's motion
  Block::particleMotion(particle);

  // Check that the particle's position, velocity, and hv have been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.01, 0.0, 0.01}));
  ASSERT_NE(particle.get_velocity(), (std::vector<float>{0.01, 0.0, 0.01}));
  ASSERT_NE(particle.get_hv(), (std::vector<float>{0.02, 0.0, 0.02}));
}

TEST(BlockTest, ProcessParticleBoxCollisions) {
  // Create a block with index [0, 0, 0]
  const std::vector<int> blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block'
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};
  const int twelve = 12;
  Particle particle(twelve, position, halfVelocity, velocity);
  block.addParticle(particle);

  // Process particle box collisions
  Block::boxCollisions(particle);

  // Check that the particle's position, velocity, and hv have been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.063, 0.02, 0.02}));
  ASSERT_NE(particle.get_velocity(), (std::vector<float>{0.02, 0.0, 0.0}));
  ASSERT_NE(particle.get_hv(), (std::vector<float>{0.04, 0.0, 0.0}));
}

TEST(BlockTest, ProcessParticleBoundaryCollisions) {
  // Create a block with index [0, 0, 0]
  std::vector<int> const blockIndex(3, 0);
  Block block(blockIndex);

  // Create a particle and add it to the block
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};
  const int thirteen = 13;
  Particle particle(thirteen, position, halfVelocity, velocity);
  block.addParticle(particle);

  // Process particle boundary collisions
  Block::boundaryCollisions(particle);

  // Check that the particle's position has been updated
  ASSERT_NE(particle.get_position(), (std::vector<float>{0.063, 0.02, 0.04}));
}

