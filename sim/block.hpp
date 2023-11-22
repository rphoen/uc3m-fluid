#ifndef BLOCK_CPP
#define BLOCK_CPP

#include "constants.hpp"
#include "particle.hpp"
#include <cmath>
#include <utility>
#include <vector>

// Block class
class Block {
public:
  // Constructor
  explicit Block(std::vector<int> index);

  // Delete the copy constructor and copy assignment operator
  Block(const Block &) = default;
  Block &operator=(const Block &) = delete;

  // Delete the move constructor and move assignment operator
  Block(Block &&) = delete;
  Block &operator=(Block &&) = delete;

  // Member particle getter
  std::vector<Particle> getParticles();

  // Get the block's index
  [[nodiscard]] std::vector<int> get_index() const;

  // Add particle to block
  void addParticle(const Particle &part);

  // Add an adjacent block to the block's adjacent block vector
  void addAdjacentBlock(const Block &adjBlock);

  // Increasing density...
  void incDensity(Particle &part, double slSq, double slSixth,
                  double densTransConstant);

  // Distance formula ..
  static double findDistance(const Particle &iPart, const Particle &jPart);

  // Transferring accelerations
  void accelerationTransfer(Particle part, double slSq,
                            double accTransConstant1, double accTransConstant2);

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
