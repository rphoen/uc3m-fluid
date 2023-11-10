

class Fluid {
public:
  Fluid(const std::string& input_file_name, const std::string& output_file_name);

  void Simulate(int iterations);

private:
  // Reads the fluid state from the input file.
  void ReadInputFile();

  // Writes the fluid state to the output file.
  void WriteOutputFile();

  // Simulates a single time step.
  void SimulateStep();

  // Computes the density of the fluid at a given point.
  double ComputeDensity(double x, double y, double z);

  // Computes the force on a particle due to pressure.
  Eigen::Vector3d ComputePressureForce(Particle& particle);

  // Computes the force on a particle due to viscosity.
  Eigen::Vector3d ComputeViscosityForce(Particle& particle);

  // Computes the force on a particle due to collisions.
  Eigen::Vector3d ComputeCollisionForce(Particle& particle);

  // Integrates the velocity and position of a particle over a single time step.
  void IntegrateParticle(Particle& particle);

  // Checks if a particle is inside the simulation box.
  bool IsInsideBox(Particle& particle);

  // Computes the distance between two particles.
  double ComputeDistance(Particle& particle1, Particle& particle2);

  // Structure to represent a fluid particle.
  struct Particle {
    double px; // Position x-coordinate.
    double py; // Position y-coordinate.
    double pz; // Position z-coordinate.
    double hvx; // Vector hv x-coordinate.
    double hvy; // Vector hv y-coordinate.
    double hvz; // Vector hv z-coordinate.
    double vx; // Velocity x-coordinate.
    double vy; // Velocity y-coordinate.
    double vz; // Velocity z-coordinate.
  };

  // The list of fluid particles.
  std::vector<Particle> particles_;

  // The simulation constants.
  const double radiusMultiplier;
  const double fluidDensity;
  const double stiffnessPressure;
  const double stiffnessCollision;
  const double damping;
  const double viscosity;
  const double particleSize;
  const double timeStep;

  // The simulation box bounds.
  const Eigen::Vector3d bmin_;
  const Eigen::Vector3d bmax_;
};
