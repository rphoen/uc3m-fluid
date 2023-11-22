#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
#include "constants.hpp"
#include <utility>
#include <vector>

// Particle class
class Particle {
private:
  // Member variables
  int id;
  std::vector<float> position;
  std::vector<float> hv;
  std::vector<float> velocity;
  double density;
  std::vector<double> acceleration;
  bool accelerated;

public:
  // Constructor and Destructor
  Particle(int id, std::vector<float> position, std::vector<float> hv,
           std::vector<float> velocity);

  ~Particle();

  // Copy constructors, move constructors
  Particle(const Particle& other);

  Particle& operator=(const Particle& other) = default;

  Particle(Particle&& other) = default;

  Particle& operator=(Particle&& other) = default;

  [[nodiscard]] int get_id() const;
  // Getters and setters for each variables
  std::vector<float> get_position();
  void set_position(std::vector<float> position);
  [[nodiscard]] float get_px() const;
  [[nodiscard]] float get_py() const;
  [[nodiscard]] float get_pz() const;

  std::vector<float> get_hv();
  void set_hv(std::vector<float> hv);
  [[nodiscard]] float get_hvx() const;
  [[nodiscard]] float get_hvy() const;
  [[nodiscard]] float get_hvz() const;

  std::vector<float> get_velocity();
  void set_velocity(std::vector<float> velocity);

  [[nodiscard]] double get_density() const;
  void set_density(double density);
  std::vector<double> get_acceleration();
  double get_ax();
  double get_ay();
  double get_az();
  void set_acceleration(std::vector<double>);
  [[nodiscard]] bool hasAccelerated() const;
  void updateAccBool();
};

#endif // FLUID_PARTICLE_HPP
