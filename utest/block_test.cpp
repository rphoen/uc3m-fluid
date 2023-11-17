#include <gtest/gtest.h>
#include "../sim/block.hpp"


TEST(ParticleTest, ParticleConstructor) {
  // Initialize particle values
  float px = 0.123;
  float py = 0.456;
  float pz = 0.789;
  float hvx = 0.012;
  float hvy = 0.034;
  float hvz = 0.056;
  float vx = 0.078;
  float vy = 0.090;
  float vz = 0.102;

  // Create a particle using the constructor
  Particle particle(px, py, pz, hvx, hvy, hvz, vx, vy, vz);

  // Verify that the particle's position, half-velocity, and velocity vectors are set correctly
  ASSERT_EQ(particle.position[0], px);
  ASSERT_EQ(particle.position[1], py);
  ASSERT_EQ(particle.position[2], pz);

  ASSERT_EQ(particle.hv[0], hvx);
  ASSERT_EQ(particle.hv[1], hvy);
  ASSERT_EQ(particle.hv[2], hvz);

  ASSERT_EQ(particle.velocity[0], vx);
  ASSERT_EQ(particle.velocity[1], vy);
  ASSERT_EQ(particle.velocity[2], vz);

  // Verify that the particle's density and acceleration are initialized correctly
  ASSERT_EQ(particle.density, 0.0);
  ASSERT_EQ(particle.acceleration[0], Constants::externalAcceleration[0]);
  ASSERT_EQ(particle.acceleration[1], Constants::externalAcceleration[1]);
  ASSERT_EQ(particle.acceleration[2], Constants::externalAcceleration[2]);
}

TEST(BoxTest, FindBlock) {
  // Set up particle position
  Particle particle = Particle(1, 0.01, 0.01, 0, 0, 0, 0, 0, 0);

  // Update box parameters
  Box::update_box_params();

  // Find the block the particle belongs to
  std::vector<int> block = Box::findBlock(particle);

  // Verify that the block coordinates are correct
  ASSERT_EQ(block[0], 1);
  ASSERT_EQ(block[1], 1);
  ASSERT_EQ(block[2], 1);
}

TEST(FormulasTest, IncDensity) {
  // Create two particles with different positions
  Particle iPart = Particle(0.1, 0.2, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
  Particle jPart = Particle(0.4, 0.5, 0.6, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);

  // Set the initial density of iPart to 0
  //   iPart.density = 0.0;

  // Call the incDensity() function to update the density of iPart
  Formulas::incDensity(iPart, jPart);

  // Verify that the density of iPart has increased
  ASSERT_GT(iPart.density, 0.0);
}

TEST(FormulasTest, FindDistance) {
  // Create two particles with different positions
  Particle iPart = Particle(0.1, 0.2, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
  Particle jPart = Particle(0.4, 0.5, 0.6, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);;

  // Calculate the distance between the two particles
  double distance = Formulas::findDistance(iPart, jPart);

  // Verify that the distance is positive
  ASSERT_GT(distance, 0.0);
}

TEST(FormulasTest, AccelerationTransfer) {
  // Create two particles with different positions
  Particle iPart = Particle(0.1, 0.2, 0.3, 0, 0, 0, 0, 0, 0.1);;
  Particle jPart = Particle(0.1, 0.2, 0.29, 0, 0, 0.1, 0, 0, 0.1);;

  // Set the initial acceleration of iPart and jPart to 0
//   iPart.acceleration = 0.0;
//   jPart.acceleration = 0.0;

  // Call the accelerationTransfer() function to update the acceleration of iPart and jPart
  Formulas::accelerationTransfer(iPart, jPart);

  // Verify that the acceleration of iPart and jPart has changed
  ASSERT_NE(iPart.acceleration[0], 0.0);
  ASSERT_NE(jPart.acceleration[0], 0.0);
}

TEST(FormulasTest, ParticleMotion) {
  // Create a particle with initial position, half-velocity, and velocity
  Particle particle = Particle(0.1, 0.2, 0.3, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06);

    // Update the particle's position, half-velocity, and velocity using the particleMotion() function
    Formulas::particleMotion(particle);

    // Verify that the particle's position, half-velocity, and velocity have changed
    ASSERT_NE(particle.position[0], 0.1);
    ASSERT_NE(particle.position[1], 0.2);
    ASSERT_NE(particle.position[2], 0.3);

    ASSERT_NE(particle.hv[0], 0.01);
    ASSERT_NE(particle.hv[1], 0.02);
    ASSERT_NE(particle.hv[2], 0.03);

    ASSERT_NE(particle.velocity[0], 0.04);
    ASSERT_NE(particle.velocity[1], 0.05);
    ASSERT_NE(particle.velocity[2], 0.06);

}

TEST(FormulasTest, BoxCollisions) {
    // Create a particle with a position inside the box
    // Set the particle's half-velocity and velocity in the x-direction
    Particle particle = Particle(0.2, 0.3, 0.4, 0.1, 0, 0, 0.2, 0, 0);

    // Call the boxCollisions() function to handle collisions with the box
    Formulas::boxCollisions(particle);

    // Verify that the particle's half-velocity and velocity in the x-direction have changed due to collisions
    ASSERT_NE(particle.hv[0], 0.1);
    ASSERT_NE(particle.velocity[0], 0.2);
}

TEST(FormulasTest, BoundaryCollisions) {
    // Create a particle with a position close to a boundary
    Particle particle = Particle(0.01, 0.2, 0.3, 0.1, 0, 0, 0.2, 0, 0);
    particle.position[0] = 0.01;
    particle.position[1] = 0.2;
    particle.position[2] = 0.3;

    // Set the particle's half-velocity and velocity in the x-direction
    particle.hv[0] = 0.1;
    particle.velocity[0] = 0.2;

    // Call the boundaryCollisions() function to handle collisions with the boundaries
    Formulas::boundaryCollisions(particle);

    // Verify that the particle's position, half-velocity, and velocity in the x-direction have changed due to collisions
    ASSERT_NE(particle.position[0], 0.01);
    ASSERT_NE(particle.hv[0], 0.1);
    ASSERT_NE(particle.velocity[0], 0.2);
}