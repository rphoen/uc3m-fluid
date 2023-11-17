#ifndef GRID_HPP
#define GRID_HPP
#include "block.hpp"
#include <unordered_map>

// Grid class
class Grid {
private:
  // All the blocks in the grid
  std::unordered_map<std::vector<int>, Block> blocks;

  // Information from initial file
  float ppm;
  int np;

  double particleMass;
  double smoothingLength;

  // Calculating the number of blocks in each dimension...
  double numberX;
  double numberY;
  double numberZ;
  // Total number of blocks in the grid
  double numBlocks;

  // Calculating the size of grid blocks in each dimension...
  double sizeX;
  double sizeY;
  double sizeZ;

  // Simulation parameters
  double slSq;
  double slCu;
  double slSixth;
  double slNinth;

  double densTransConstant;
  double accTransConstant1;
  double accTransConstant2;

public:
  // Constructor and Destructor
  Grid(float ppm, int np);
  ~Grid();

  // Getters and setters for each variable
  std::unordered_map<std::vector<int>, Block> get_blocks();

  float get_ppm();
  int get_np();

  double get_particleMass();
  double get_smoothingLength();

  double get_numberX();
  double get_numberY();
  double get_numberZ();

  double get_numBlocks();

  double get_sizeX();
  double get_sizeY();
  double get_sizeZ();

  double get_slSq();
  double get_slCu();
  double get_slSixth();
  double get_slNinth();

  double get_densTransConstant();
  double get_accTransConstant1();
  double get_accTransConstant2();

  // Update variables
  void update_grid();

  // Find the block that a particle belongs in
  std::vector<int> findBlock(float px, float py, float pz);
};

#endif // GRID_HPP
