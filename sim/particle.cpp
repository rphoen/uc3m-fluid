// Particle class implementation
#include "particle.hpp"
#include "constants.hpp"

// Constructor
Particle::Particle(std::vector<float> position, std::vector<float> hv,
                   std::vector<float> velocity) {
  // Initialize position, half-velocity, and velocity vectors
  position = position;
  hv = hv;
  velocity = velocity;

  // Initialize other member variables
  density = 0.0;
  acceleration = Constants::externalAcceleration;
}

// Destructor
Particle::~Particle() {
  // Destructor code, if needed
}

// Getters and setters for each variables
std::vector<float> Particle::get_position() { return position; }
void Particle::set_position(std::vector<float> position) {
  position = position;
}
float Particle::get_px() { return position[0]; }
float Particle::get_py() { return position[1]; }
float Particle::get_pz() { return position[2]; }

std::vector<float> Particle::get_hv() { return hv; }
void Particle::set_hv(std::vector<float> hv) { hv = hv; }
float Particle::get_hvx() { return hv[0]; }
float Particle::get_hvy() { return hv[1]; }
float Particle::get_hvz() { return hv[2]; }

std::vector<float> Particle::get_velocity() { return velocity; }
void Particle::set_velocity(std::vector<float> velocity) {
  velocity = velocity;
}
float Particle::get_vx() { return velocity[0]; }
float Particle::get_vy() { return velocity[1]; }
float Particle::get_vz() { return velocity[2]; }

double Particle::get_density() { return density; }
std::vector<double> Particle::get_acceleration() { return acceleration; }
double Particle::get_ax() { return acceleration[0]; }
double Particle::get_ay() { return acceleration[1]; }
double Particle::get_az() { return acceleration[2]; }

void set_density(double density) { density = density; }
