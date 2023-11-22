#include "progargs.hpp"

int progargs(int argc, std::array<char *, 4> argv) {
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << ".\n";
    return -1;
  }
  if (std::stoi(argv[1]) == 0) {
    std::cout << "Error: Time steps must be numeric."
              << "\n";
    return -1;
  }
  if (std::stoi(argv[1]) <= 0) {
    std::cout << "Error: Invalid number of time steps."
              << "\n";
    return -2;
  }
  auto checkFile = [](const char *filename, const char *mode, int errorCode) {
    std::ifstream const file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Cannot open " << filename << " for " << mode << "\n";
      return errorCode;
    }
    return 0;
  };

  return checkFile(argv[2], "reading", -3) + checkFile(argv[3], "writing", -4);
}
