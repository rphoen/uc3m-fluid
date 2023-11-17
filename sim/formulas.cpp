#include "formulas.hpp"
#include "constants.hpp"

namespace Formulas {

  // Increasing density...
  void incDensity(Particle iPart, Particle jPart) {
    float ix = iPart.position[0];
    float iy = iPart.position[1];
    float iz = iPart.position[2];
    float jx = jPart.position[0];
    float jy = jPart.position[1];
    float jz = jPart.position[2];

    // Avoid nesting, possible future hotspot
    // Sqrt and ^2 cancel
    double vecNorm =
        pow(sqrt(pow((ix - jx), 2) + pow((iy - jy), 2) + pow((iz - jz), 2)), 2);
    if (vecNorm < Constants::slSq) {
      double densityChange = pow((Constants::slSq - vecNorm), 3);
      iPart.density += densityChange;
      iPart.density =
          (iPart.density + Constants::slSixth) * Constants::densTransConstant;
    } else if (vecNorm >= Constants::slSq) {
      iPart.density += 0;
    }
  };

  // Formula to calculate the distance between two particles
  double findDistance(Particle iPart, Particle jPart) {
    float ix = iPart.position[0];
    float iy = iPart.position[1];
    float iz = iPart.position[2];
    float jx = jPart.position[0];
    float jy = jPart.position[1];
    float jz = jPart.position[2];

    double vecNorm =
        pow(sqrt(pow((ix - jx), 2) + pow((iy - jy), 2) + pow((iz - jz), 2)), 2);

    double distance = sqrt(fmax(vecNorm, pow(10, -12)));
    return distance;
  };

  // Need to make sure acceleration transfers aren't double counted...
  void accelerationTransfer(Particle iPart, Particle jPart) {
    double distance = findDistance(iPart, jPart);

    for (int i = 0; i < 3; i++) {
      double accChange =
          // Either too little precision, or a computational error
            // Rewrite the code
          ((((iPart.position[i] - jPart.position[i]) *
             Constants::accTransConstant1 *
             (pow((Constants::smoothingLength - distance), 2)) / distance) *
            (iPart.density + jPart.density - (2 * Constants::fluidDensity)) *
            Constants::accTransConstant2) /
           (iPart.density * jPart.density));

      // Extra variable inside particles, a boolean for example
      iPart.acceleration[i] += accChange;
      jPart.acceleration[i] -= accChange;
    }
  };

  void particleMotion(Particle iPart) {
    for (int i = 0; i < 3; i++) {
      iPart.position[i] =
          iPart.position[i] + iPart.hv[i] * Constants::timeStep + iPart.acceleration[i] * pow(Constants::timeStep, 2);
      iPart.velocity[i] = iPart.hv[i] + ((iPart.acceleration[i] * Constants::timeStep) / 2);
      iPart.hv[i] = iPart.hv[i] + iPart.acceleration[i] * Constants::timeStep;
    }
  };

  void boxCollisions(Particle iPart) {
    // Checking x-boundary collisions...
    float newX = iPart.position[0] + iPart.hv[0] * Constants::timeStep;

    // Check for both bounds...
    double changeXLower =
        Constants::particleSize - (newX - Constants::boxLowerBound[0]);
    double changeXUpper =
        Constants::particleSize - (newX - Constants::boxUpperBound[0]);

    if (changeXLower > pow(10, -10)) {
      iPart.acceleration[0] += Constants::stiffnessCollisions * changeXLower -
                               Constants::damping * iPart.velocity[0];
    } else if (changeXUpper > pow(10, -10)) {
      iPart.acceleration[0] -= Constants::stiffnessCollisions * changeXLower +
                               Constants::damping * iPart.velocity[0];
    };
    // Checking y-boundary collisions...
    float newY = iPart.position[1] + iPart.hv[1] * Constants::timeStep;

    // Check for both bounds...
    double changeYLower =
        Constants::particleSize - (newY - Constants::boxLowerBound[1]);
    double changeYUpper =
        Constants::particleSize - (newY - Constants::boxUpperBound[1]);

    if (changeYLower > pow(10, -10)) {
      iPart.acceleration[1] += Constants::stiffnessCollisions * changeYLower -
                               Constants::damping * iPart.velocity[1];
    } else if (changeYUpper > pow(10, -10)) {
      iPart.acceleration[1] -= Constants::stiffnessCollisions * changeYLower +
                               Constants::damping * iPart.velocity[1];
    };

    // Checking z-boundary collisions...
    float newZ = iPart.position[2] + iPart.hv[2] * Constants::timeStep;

    // Check for both bounds...
    double changeZLower =
        Constants::particleSize - (newZ - Constants::boxLowerBound[2]);
    double changeZUpper =
        Constants::particleSize - (newZ - Constants::boxUpperBound[2]);

    if (changeZLower > pow(10, -10)) {
      iPart.acceleration[2] += Constants::stiffnessCollisions * changeZLower -
                               Constants::damping * iPart.velocity[2];
    } else if (changeZUpper > pow(10, -10)) {
      iPart.acceleration[2] -= Constants::stiffnessCollisions * changeZLower +
                               Constants::damping * iPart.velocity[2];
    };
  };

  void boundaryCollisions(Particle iPart) {
    // X-boundary...
    float dXLower = iPart.position[0] - Constants::boxLowerBound[0];
    float dXUpper = Constants::boxUpperBound[0] - iPart.position[0];

    if (dXLower < 0) {
      iPart.position[0] = Constants::boxLowerBound[0] - dXLower;
      iPart.velocity[0] = -1 * iPart.velocity[0];
      iPart.hv[0] = -1 * iPart.hv[0];
    } else if (dXUpper < 0) {
      iPart.position[0] = Constants::boxUpperBound[0] + dXUpper;
      iPart.velocity[0] = -1 * iPart.velocity[0];
      iPart.hv[0] = -1 * iPart.hv[0];
    }

    // Y-boundary...
    float dYLower = iPart.position[1] - Constants::boxLowerBound[1];
    float dYUpper = Constants::boxUpperBound[1] - iPart.position[1];

    if (dYLower < 0) {
      iPart.position[1] = Constants::boxLowerBound[1] - dYLower;
      iPart.velocity[1] = -1 * iPart.velocity[1];
      iPart.hv[1] = -1 * iPart.hv[1];
    } else if (dYUpper < 0) {
      iPart.position[1] = Constants::boxUpperBound[1] + dYUpper;
      iPart.velocity[1] = -1 * iPart.velocity[1];
      iPart.hv[1] = -1 * iPart.hv[1];
    }

    // Z-boundary...
    float dZLower = iPart.position[2] - Constants::boxLowerBound[2];
    float dZUpper = Constants::boxUpperBound[2] - iPart.position[2];

    if (dZLower < 0) {
      iPart.position[2] = Constants::boxLowerBound[2] - dZLower;
      iPart.velocity[2] = -1 * iPart.velocity[2];
      iPart.hv[2] = -1 * iPart.hv[2];
    } else if (dZUpper < 0) {
      iPart.position[2] = Constants::boxUpperBound[2] + dZUpper;
      iPart.velocity[2] = -1 * iPart.velocity[2];
      iPart.hv[2] = -1 * iPart.hv[2];
    }
  };
}; // namespace Formulas

