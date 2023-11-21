#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "grid.hpp"
#include "particle.hpp"
#include <cmath>
#include <vector>

  // Block class
  class Block {
  public:
    // Constructor
    explicit Block(std::vector<int> index, Grid* simGrid);

    // Member particle getter
    std::vector<Particle> getParticles();

    // Add particle to block
    void addParticle(const Particle& part);

    // Finds adjacent blocks
    void findAdjBlocks(auto numX, auto numY, auto numZ, Grid* simGrid);

    // Increasing density...
    void incDensity(Particle& part, const Grid& grid);

    // Distance formula ..
    static double findDistance(const Particle& iPart, const Particle& jPart);

    // Transferring accelerations
    void accelerationTransfer(Particle part, const Grid& grid);

    // Particle motion
    static void particleMotion(Particle part);

    // Process box collisions
    static void boxCollisions(Particle part);

    // Process boundary collisions
    static void boundaryCollisions(Particle part);

    // Destructor
    ~Block();

  private:
    std::vector<Particle> particles;
    std::vector<Block> adjBlocks;
    std::vector<int> index;
};

#endif
