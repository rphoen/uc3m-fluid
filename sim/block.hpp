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
    explicit Block(std::vector<int> blockIndex);

    // Member particle getter
    std::vector<Particle> getParticles();

    // Get the block's index
    [[nodiscard]] std::vector<int> get_index() const;

    // Add particle to block
    void addParticle(const Particle& part);

    // Add an adjacent block to the block's adjacent block vector
    void addAdjacentBlock(const Block& adjBlock);

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
