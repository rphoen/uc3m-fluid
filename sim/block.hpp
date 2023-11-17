#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "particle.hpp"
#include <cmath>
#include <vector>

  // Block class
  class Block {
  public:
    // Constructor
    Block(std::vector<int> index);

    // Member particle getter
    std::vector<Particle> getParticles();

    // Add particle to block
    void addParticle(const Particle& part);

    // Get index
    std::vector<int> getIndex();

    // Increasing density...
    void incDensity(const Particle& part, Grid grid);

    // Distance formula ..
    double findDistance(Particle iPart, Particle jPart);

    // Transferring accelerations
    void accelerationTransfer();

    // Particle motion
    void particleMotion();

    // Process box collisions
    void boxCollisions();

    // Process boundary collisions
    void boundaryCollisions();

    // Destructor
    ~Block();

  private:
    std::vector<Particle> particles;
    std::vector<Block> adjBlocks;
    std::vector<int> index;
};

#endif
