// Need to create a function that will do the simulation for ONE iteration...
#include "simulation.hpp"
// #include "grid.hpp"
// #include "block.hpp"

// What arguments?
  // One particle, and then we can check its block info for adjacent particles?
void simulateOneStep(const Grid& simGrid) // Why by reference?
{
  std::unordered_map<std::vector<int>, Block> blocksDict = simGrid.get_blocks();
      for (const auto& blockPair : blocksDict) {
        auto blockObj = blockPair.second;
        for (auto particle : blockObj.getParticles()) {
          // Run each member function of a block on the particle in question
          blockObj.accelerationTransfer(particle);
          blockObj.incDensity(particle);
          blockObj.boxCollisions(particle);
          blockObj.particleMotion(particle);
          blockObj.boundaryCollisions(particle);
        }
      }
}

// Do I need to verify particle blocks?
//
//         part.updateBlock(part.position[0], part.position[1], part.position[2]); // Probably should write an update_block field in particle class
//            adjParticle = findBlock(adjParticle);