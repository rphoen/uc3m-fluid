#include "gtest/gtest.h"
#include "../sim/particle.hpp"

TEST(ParticleTest, ConstructorTest) {
  // Create a particle with id 1, position (0.1, 0.2, 0.3), half-velocity (0.4, 0.5, 0.6), and velocity (0.2, 0.4, 0.6)
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};
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
  std::vector<float> const position{0.1, 0.2, 0.3};
  std::vector<float> const halfVelocity{0.1, 0.2, 0.3};
  std::vector<float> const velocity{0.2, 0.4, 0.6};
  Particle particle(2, position, halfVelocity, velocity);

  // Set the particle's ID
  ASSERT_EQ(particle.get_id(), 2);

  // Set the particle's position
  std::vector<float> const position2{0.4, 0.5, 0.6};
  particle.set_position(position2);
  ASSERT_EQ(particle.get_position(), position2);

  // Set the particle's half-velocity
  std::vector<float> const halfVelocity2{0.2, 0.3, 0.4};
  particle.set_hv(halfVelocity2);
  ASSERT_EQ(particle.get_hv(), halfVelocity2);

  // Set the particle's velocity
  std::vector<float> const velocity2{0.4, 0.6, 0.8};
  particle.set_velocity(velocity2);
  ASSERT_EQ(particle.get_velocity(), velocity2);

  // Set the particle's density
  const double thirt = 13.5;
  particle.set_density(thirt);
  ASSERT_EQ(particle.get_density(), 13.5);

  // Set the particle's acceleration
  std::vector<double> const acceleration = {0.1, 0.1, 0.1};
  particle.set_acceleration(acceleration);
  ASSERT_EQ(particle.get_acceleration(), acceleration);

  // Mark the particle as accelerated
  particle.updateAccBool();
  ASSERT_TRUE(particle.hasAccelerated());
}
