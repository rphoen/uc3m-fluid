#ifndef GRID_HPP
#define GRID_HPP
#include "block.hpp"
#include "constants.hpp"
#include "hash.cpp"
#include <iostream>
#include <ostream>
#include <unordered_map>

// Grid class
class Grid {
private:
  // All the blocks in the grid
  std::unordered_map<std::vector<int>, Block, hashing::vHash> blocks;

  // Information from initial file and the simulation constants that depend on
  // them
  float ppm;
  int np;
  int count{}; // number of particles counted

  // Number of blocks in each dimension
  double numberX{};
  double numberY{};
  double numberZ{};
  std::vector<double> numberVector;

  // The size of grid blocks in each dimension
  double sizeX{};
  double sizeY{};
  double sizeZ{};
  std::vector<double> sizesVector;

  // Simulation parameters
  double particleMass{};
  double smoothingLength{};

  // Variations of smoothing length that are useful for the simulation
  double slSq{};
  double slCu{};
  double slSixth{};
  double slNinth{};

  // Constants for density and acceleration formula
  double densTransConstant{};
  double accTransConstant1{};
  double accTransConstant2{};

public:
  // Constructor and Destructor
  explicit Grid(float ppm, int np);
  ~Grid();

  // Delete the copy constructor and copy assignment operator
  Grid(const Grid &) = delete;
  Grid &operator=(const Grid &) = delete;

  // Delete the move constructor and move assignment operator
  Grid(Grid &&) = default;
  Grid &operator=(Grid &&) = delete;

  // Getters and setters for each variable
  [[nodiscard]] std::unordered_map<std::vector<int>, Block, hashing::vHash>
  get_blocks() const;

  [[nodiscard]] float get_ppm() const;
  [[nodiscard]] int get_np() const;

  [[nodiscard]] int get_count() const;
  void set_count(int count);

  [[nodiscard]] double get_particleMass() const;
  [[nodiscard]] double get_smoothingLength() const;

  [[nodiscard]] double get_numberX() const;
  [[nodiscard]] double get_numberY() const;
  [[nodiscard]] double get_numberZ() const;

  [[nodiscard]] double get_sizeX() const;
  [[nodiscard]] double get_sizeY() const;
  [[nodiscard]] double get_sizeZ() const;

  [[nodiscard]] double get_slSq() const;
  [[nodiscard]] double get_slCu() const;
  [[nodiscard]] double get_slSixth() const;
  [[nodiscard]] double get_slNinth() const;

  [[nodiscard]] double get_densTransConstant() const;
  [[nodiscard]] double get_accTransConstant1() const;
  [[nodiscard]] double get_accTransConstant2() const;

  // Finds adjacent blocks
  void findAdjBlocks(Block centerBlock) const;

  // block functions
  void add_particle_to_block(const Particle &p);

  // Update variables
  void update_grid();

  // Find the block that a particle belongs in
  std::vector<int> findBlock(Particle part);

  // Helper function for findBlock
  static std::vector<float> moveParticleInBounds(std::vector<float> position);
};

#endif // GRID_HPP
