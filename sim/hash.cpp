#include <cstdint>
#include <vector>

int const six = 6;
const uint32_t GoldenRatioInitValue = 0x9e3779b9;
// Hashing function in order to use vector<int> as key for unordered map
namespace hashing {
  struct vHash {
      auto operator() (const std::vector<int> &vector) const {
        auto hash = vector.size();
        for (const auto &element : vector) {
          hash ^= element + GoldenRatioInitValue + (hash << six) + (hash >> 2);
        }
        return hash;
      }
  };
}