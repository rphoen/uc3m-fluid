// Need to create a function that will do the simulation for ONE iteration...
#include "block.hpp"
#include "formulas.hpp"

// What arguments?
  // One particle, and then we can check its block info for adjacent particles?
void simulateOneStep(Particle part) // Why by reference?
{
      for (auto adjBlock : part.adjBlocks) {
        for (auto adjParticle : adjBlock.particles) {
          part.updateBlock(part.position[0], part.position[1], part.position[2]); // Probably should write an update_block field in particle class
          adjParticle = findBlock(adjParticle);

          // Make these member functions of block
          Formulas::accelerationTransfer(part, adjParticle);
          Formulas::incDensity(part, adjParticle);

          Formulas::boxCollisions(part);
          Formulas::particleMotion(part);
          Formulas::boundaryCollisions(part);


          part.updateBlock(part.position[0], part.position[1], part.position[2]);
          adjParticle.updateBlock(adjParticle.position[0], adjParticle.position[1], adjParticle.position[2]);
        }
      }
};
