#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "particle.hpp"
#include <cmath>
#include <vector>
#include <unordered_map>

  // Block class
  class Block {
  public:
    std::vector<Particle> particles;
    std::vector<Block> adjBlocks;

  private:
};

#endif
