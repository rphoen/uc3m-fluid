#include "gtest/gtest.h"
#include "../sim/particle.hpp"
#include "../sim/constants.hpp"

// g++ -o utest/particle_info_test utest/particle_info_test.cpp sim/particle.cpp -lgtest -lpthread

TEST(ParticleTest, ConstructorTest) {
  // Create a particle with id 1, position (0.1, 0.2, 0.3), half-velocity (0.4, 0.5, 0.6), and velocity (0.2, 0.4, 0.6)
  std::vector<float> position{0.1, 0.2, 0.3};
  std::vector<float> halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> velocity{0.2, 0.4, 0.6};
  Particle particle(1, position, halfVelocity, velocity);

  // Check that the particle's ID is set correctly
  ASSERT_EQ(particle.get_id(), 1);

  // Check that the particle's position is set correctly
  ASSERT_EQ(particle.get_position(), position);

  // Check that the particle's half-velocity is set correctly
  ASSERT_EQ(particle.get_hv(), halfVelocity);

  // Check that the particle's velocity is set correctly
  ASSERT_EQ(particle.get_velocity(), velocity);

  // Check that the particle's density is initialized to 0.0
  ASSERT_EQ(particle.get_density(), 0.0);

  // Check that the particle's acceleration is set to the external acceleration
  ASSERT_EQ(particle.get_acceleration(), Constants::getExternalAcceleration());

  // Check that the particle's accelerated flag is initialized to false
  ASSERT_FALSE(particle.hasAccelerated());
}

TEST(ParticleTest, GettersAndSettersTest) {
  // Create a particle
  Particle particle(2, {0.1, 0.2, 0.3}, {0.1, 0.2, 0.3}, {0.2, 0.4, 0.6});

  // Set the particle's ID
  ASSERT_EQ(particle.get_id(), 2);

  // Set the particle's position
  std::vector<float> position{0.4, 0.5, 0.6};
  particle.set_position(position);
  ASSERT_EQ(particle.get_position(), position);

  // Set the particle's half-velocity
  std::vector<float> halfVelocity{0.2, 0.3, 0.4};
  particle.set_hv(halfVelocity);
  ASSERT_EQ(particle.get_hv(), halfVelocity);

  // Set the particle's velocity
  std::vector<float> velocity{0.4, 0.6, 0.8};
  particle.set_velocity(velocity);
  ASSERT_EQ(particle.get_velocity(), velocity);

  // Set the particle's density
  particle.set_density(13.5);
  ASSERT_EQ(particle.get_density(), 13.5);

  // Set the particle's acceleration
  std::vector<double> acceleration = {0.1, 0.1, 0.1};
  particle.set_acceleration(acceleration);
  ASSERT_EQ(particle.get_acceleration(), acceleration);

  // Mark the particle as accelerated
  particle.updateAccBool();
  ASSERT_TRUE(particle.hasAccelerated());
}
