#include "block.hpp"

// Constructor for the Block class
Block::Block(std::vector<int> blockIndex) : particles({}), adjBlocks({}), index(std::move(blockIndex)) {
}

// Return a vector of all particles that belong to a specific block
std::vector<Particle> Block::getParticles() { return particles; }

// Return the block's index
std::vector<int> Block::get_index() const { return index; }

// Add a particle to the vector of all particles belonging to a specific block
void Block::addParticle(const Particle &part) { particles.emplace_back(part); }

// Add an adjacent block to the block's adjacent block vector
void Block::addAdjacentBlock(const Block &adjBlock) {
  adjBlocks.emplace_back(adjBlock);
}

// Increasing density between a given particle and every particle in the
// adjacent blocks
void Block::incDensity(Particle &part, double slSq, double slSixth,
                       double densTransConstant) {
  auto posx = part.get_px();
  auto posy = part.get_py();
  auto posz = part.get_pz();

  for (auto &blk : adjBlocks) {
    auto adjParts = blk.getParticles();
    for (const auto &adjPart : adjParts) {
      auto px2 = adjPart.get_px();
      auto py2 = adjPart.get_py();
      auto pz2 = adjPart.get_pz();
      auto xDiffSq = pow((posx - px2), 2);
      auto yDiffSq = pow((posy - py2), 2);
      auto zDiffSq = pow((posz - pz2), 2);
      auto diffSum = xDiffSq + yDiffSq + zDiffSq;

      if (diffSum < slSq) {
        double const densityChange = pow((slSq - diffSum), 3);
        double const newDensity = part.get_density() + densityChange;
        double const densTransformation = (newDensity + slSixth) * densTransConstant;
        part.set_density(densTransformation);
      }
    }
  }
}

const int ten = 10;
const int minus_twelve = -12;

// Formula to calculate the distance between two given particles
double Block::findDistance(const Particle &iPart, const Particle &jPart) {
  float const ipx = iPart.get_px();
  float const ipy = iPart.get_py();
  float const ipz = iPart.get_pz();
  float const jpx = jPart.get_px();
  float const jpy = jPart.get_py();
  float const jpz = jPart.get_pz();

  auto xDiffSq = pow((ipx - jpx), 2);
  auto yDiffSq = pow((ipy - jpy), 2);
  auto zDiffSq = pow((ipz - jpz), 2);
  auto diffSum = xDiffSq + yDiffSq + zDiffSq;

  double const distance = sqrt(fmax(diffSum, pow(ten, minus_twelve)));
  return distance;
}

// Transfer accelerations between a given particle and every particle in the
// adjacent blocks NEED TO MAKE SHORTER
void Block::accelerationTransfer(Particle part, double slSq,
                                 double accTransConstant1,
                                 double accTransConstant2) {
  auto px1 = part.get_px();
  auto py1 = part.get_py();
  auto pz1 = part.get_pz();

  for (auto &block : adjBlocks) {
    auto adjParts = block.getParticles();
    for (auto &adjPart : adjParts) {
      auto px2 = adjPart.get_px();
      auto py2 = adjPart.get_py();
      auto pz2 = adjPart.get_pz();

      // Need to check if distance is short enough first
      auto xDiffSq = pow((px1 - px2), 2);
      auto yDiffSq = pow((py1 - py2), 2);
      auto zDiffSq = pow((pz1 - pz2), 2);
      auto diffSum = xDiffSq + yDiffSq + zDiffSq;

      if (diffSum < slSq) // Update the acceleration
      {
        double const distance = findDistance(part, adjPart);
        // Change this to helper function eventually
        auto xAccChange = (((px1 - px2) * accTransConstant1 *
                            ((pow((slSq - distance), 2)) / distance) *
                            (part.get_density() + adjPart.get_density() -
                             (2 * Constants::fluidDensity)) *
                            accTransConstant2) /
                           (part.get_density() * adjPart.get_density()));

        auto yAccChange = (((py1 - py2) * accTransConstant1 *
                            ((pow((slSq - distance), 2)) / distance) *
                            (part.get_density() + adjPart.get_density() -
                             (2 * Constants::fluidDensity)) *
                            accTransConstant2) /
                           (part.get_density() * adjPart.get_density()));

        auto zAccChange = (((pz1 - pz2) * accTransConstant1 *
                            ((pow((slSq - distance), 2)) / distance) *
                            (part.get_density() + adjPart.get_density() -
                             (2 * Constants::fluidDensity)) *
                            accTransConstant2) /
                           (part.get_density() * adjPart.get_density()));

        if (!part.hasAccelerated()) {
          std::vector<double> const partNew = {(part.get_ax() + xAccChange),
                                         (part.get_ay() + yAccChange),
                                         (part.get_az() + zAccChange)};
          std::vector<double> const adjNew = {(adjPart.get_ax() - xAccChange),
                                        (adjPart.get_ay() - yAccChange),
                                        (adjPart.get_az() - zAccChange)};
          part.set_acceleration(partNew);
          part.updateAccBool();
          adjPart.set_acceleration(adjNew);
          adjPart.updateAccBool();
        }
      }
    }
  }
}

// Update a particle (i.e., its position, hv, and velocity
void Block::particleMotion(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> vectorhv = part.get_hv();
  std::vector<float> velocity = part.get_velocity();
  std::vector<double> acceleration = part.get_acceleration();

  for (int i = 0; i < 3; i++) {
    position[i] =
        static_cast<float>(position[i] + vectorhv[i] * Constants::timeStep +
                           acceleration[i] * pow(Constants::timeStep, 2));
    velocity[i] = static_cast<float>(
        vectorhv[i] + ((acceleration[i] * Constants::timeStep) / 2));
    vectorhv[i] = static_cast<float>(vectorhv[i] + acceleration[i] * Constants::timeStep);
  }

  part.set_position(position);
  part.set_velocity(velocity);
  part.set_hv(vectorhv);
}

const int minus_ten = -10;

// Process the box collisions of one particle
void Block::boxCollisions(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> vectorhv = part.get_hv();
  std::vector<float> velocity = part.get_velocity();
  std::vector<double> currentAcc = part.get_acceleration();
  std::vector<double> newAcc = part.get_acceleration();
  for (int i = 0; i < 3; i++) {
    auto newCoord =
        static_cast<float>(position[i] + vectorhv[i] * Constants::timeStep);
    double const changeLower =
        Constants::particleSize - (newCoord - Constants::boxLowerBound[i]);
    double const changeUpper =
        Constants::particleSize - (Constants::boxUpperBound[i] - newCoord);
    auto check = pow(ten, minus_ten);

    if (changeLower > check) {
      newAcc[i] = currentAcc[i] + Constants::stiffnessCollisions * changeLower -
                  Constants::damping * velocity[i];

    } else if (changeUpper > check) {
      newAcc[i] = currentAcc[i] - Constants::stiffnessCollisions * changeLower -
                  Constants::damping * velocity[i];
    }
  }
  part.set_acceleration(newAcc);
}

// Process the boundary collisions of one particle
void Block::boundaryCollisions(Particle part) {
  std::vector<float> position = part.get_position();
  std::vector<float> velocity = part.get_velocity();
  std::vector<float> vectorhv = part.get_hv();

  for (int i = 0; i < 3; i++) {
    auto dLower = position[i] - Constants::boxLowerBound[i];
    auto dUpper = Constants::boxUpperBound[i] - position[i];

    if (dLower < 0) {
      position[i] = static_cast<float>(Constants::boxLowerBound[i] - dLower);
      velocity[i] = -1 * velocity[i];
      vectorhv[i] = -1 * vectorhv[i];
    } else if (dUpper < 0) {
      position[i] = static_cast<float>(Constants::boxUpperBound[i] + dUpper);
      velocity[i] = -1 * velocity[i];
      vectorhv[i] = -1 * vectorhv[i];
    }
  }
  part.set_position(position);
  part.set_velocity(velocity);
  part.set_hv(vectorhv);
}

// Block destructor implementation
Block::~Block() = default;
