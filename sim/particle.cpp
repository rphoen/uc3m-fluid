// Particle class implementation
#include "particle.hpp"

#include <utility>

// Constructor
Particle::Particle(int particleID, std::vector<float> partPosition,
                   std::vector<float> partHv, std::vector<float> partVelocity) : id(particleID), position(std::move(partPosition)), acceleration(Constants::externalAcceleration), density(0.0), accelerated(false), velocity(std::move(partVelocity)), hv(std::move(partHv)) {
}

// Destructor
Particle::~Particle() = default;

Particle::Particle(const Particle& other) = default;

// Getters and setters for each variables
int Particle::get_id() const { return id; }

std::vector<float> Particle::get_position() { return position; }
void Particle::set_position(std::vector<float> newPosition) {
  position = std::move(newPosition);
}
float Particle::get_px() const { return position[0]; }
float Particle::get_py() const { return position[1]; }
float Particle::get_pz() const { return position[2]; }

std::vector<float> Particle::get_hv() { return hv; }
void Particle::set_hv(std::vector<float> newHv) { hv = std::move(newHv); }
float Particle::get_hvx() const { return hv[0]; }
float Particle::get_hvy() const { return hv[1]; }
float Particle::get_hvz() const { return hv[2]; }

std::vector<float> Particle::get_velocity() { return velocity; }
void Particle::set_velocity(std::vector<float> newVelocity) {
  velocity = std::move(newVelocity);
}

double Particle::get_density() const { return density; }
void Particle::set_density(double newDensity) { density = newDensity; }
std::vector<double> Particle::get_acceleration() { return acceleration; }
double Particle::get_ax() { return acceleration[0]; }
double Particle::get_ay() { return acceleration[1]; }
double Particle::get_az() { return acceleration[2]; }
void Particle::set_acceleration(std::vector<double> newAcc) {
  acceleration = std::move(newAcc);
}
bool Particle::hasAccelerated() const { return accelerated; }
void Particle::updateAccBool() { accelerated = true; }
