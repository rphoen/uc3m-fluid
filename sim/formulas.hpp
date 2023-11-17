#ifndef FLUID_FORMULAS_HPP
#define FLUID_FORMULAS_HPP

#include "particle.hpp"

namespace Formulas {
  // Repositioning particles...
  // ........

  // Increasing density...
  void incDensity(Particle iPart, Particle jPart);

  double findDistance(Particle iPart, Particle jPart);

  void accelerationTransfer(Particle iPart, Particle jPart);

  void particleMotion(Particle iPart);

  void boxCollisions(Particle iPart);

  void boundaryCollisions(Particle iPart);
}; // namespace Formulas


#endif  // FLUID_FORMULAS_HPP
