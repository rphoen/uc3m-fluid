// Particle class implementation
#include "particle.hpp"
#include "constants.hpp"
#include <utility>

// Constructor
Particle::Particle(std::vector<float> partPosition, std::vector<float> partHv,
                   std::vector<float> partVelocity) {
  // Initialize position, half-velocity, and velocity vectors
  position = std::move(partPosition);
  hv = std::move(partHv);
  velocity = std::move(partVelocity);
  accelerated = false;

  // Initialize other member variables
  density = 0.0;
  acceleration = Constants::externalAcceleration;
}

// Destructor
Particle::~Particle() = default;

// Getters and setters for each variables
std::vector<float> Particle::get_position() { return position; }
void Particle::set_position(std::vector<float> newPosition) {
  position = std::move(newPosition);
}
float Particle::get_px() const {return position[0];}
float Particle::get_py() const {return position[1];}
float Particle::get_pz() const {return position[2];}

std::vector<float> Particle::get_hv() { return hv; }
void Particle::set_hv(std::vector<float> newHv) { hv = std::move(newHv); }
float Particle::get_hvx() const {return hv[0];}
float Particle::get_hvy() const {return hv[1];}
float Particle::get_hvz() const {return hv[2];}

std::vector<float> Particle::get_velocity() { return velocity; }
void Particle::set_velocity(std::vector<float> newVelocity) {
  velocity = std::move(newVelocity);
}

double Particle::get_density() const { return density; }
void Particle::set_density(double newDensity) {density = newDensity;}
std::vector<double> Particle::get_acceleration() { return acceleration; }
double Particle::get_ax() { return acceleration[0]; }
double Particle::get_ay() { return acceleration[1]; }
double Particle::get_az() { return acceleration[2]; }
void Particle::set_acceleration(std::vector<double> newAcc)
{
  acceleration = std::move(newAcc);
}
bool Particle::hasAccelerated() const {return accelerated;}
void Particle::updateAccBool()
{
  accelerated = true;
}

