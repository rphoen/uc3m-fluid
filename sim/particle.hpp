#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP

#include <vector>

// Particle class
class Particle {
public:
  // Constructor and Destructor
  Particle(std::vector<float> position, std::vector<float> hv,
           std::vector<float> velocity);

  ~Particle();

  // Member variables
  std::vector<float> position;
  std::vector<float> hv;
  std::vector<float> velocity;
  double density;
  std::vector<double> acceleration;

private:
  // Getters and setters for each variables
  std::vector<float> get_position();
  void set_position(std::vector<float> position);
  float get_px();
  float get_py();
  float get_pz();

  std::vector<float> get_hv();
  void set_hv(std::vector<float> hv);
  float get_hvx();
  float get_hvy();
  float get_hvz();

  std::vector<float> get_velocity();
  void set_velocity(std::vector<float> velocity);
  float get_vx();
  float get_vy();
  float get_vz();

  double get_density();
  std::vector<double> get_acceleration();
  double get_ax();
  double get_ay();
  double get_az();
};

#endif // FLUID_PARTICLE_HPP
