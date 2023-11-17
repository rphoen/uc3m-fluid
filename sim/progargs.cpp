#include <fstream>
#include <iostream>

int progargs(int argc, char **argv) {

  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "."
              << std::endl;
    return -1;
  } else {
    // check if nts is integer, if not error code -1
    if (atoi(argv[1]) == 0) {
      std::cout << "Error: Time steps must be numeric." << std::endl;
      return -1;
    }
    // check if nts is negative
    if (atoi(argv[1]) <= 0) {
      std::cout << "Error: Invalid number of time steps." << std::endl;
      return -2;
    }

    // check if input file can be opened for reading
    std::ifstream infile(argv[2]);
    if (infile.is_open()) {
      infile.close();
    } else {
      std::cerr << "Error: Cannot open " << argv[2] << " for reading"
                << std::endl;
      return -3;
    }

    // check if input file can be opened for reading
    std::ifstream outfile(argv[3]);
    if (outfile.is_open()) {
      outfile.close();
    } else {
      std::cerr << "Error: Cannot open " << argv[3] << " for writing\n";
      return -4;
    }

    return 0;
  }
}
