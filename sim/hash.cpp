#include <vector>

// Hashing function in order to use vector<int> as key for unordered map
namespace hashing {
  struct vHash {
      auto operator() (const std::vector<int> &vector) const {
        auto hash = vector.size();
        for (auto &element : vector) {
          hash ^= element + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
      }
  };
}