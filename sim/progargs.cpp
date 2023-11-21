#include <fstream>
#include <iostream>

int progargs(int argc, std::array<char*, 4> argv) {
  if (argc != 4) { // Check that there are 4 arguments
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "." << "\n";
    return -1;}
    if (std::stoi(argv[1]) == 0) { // Check if nts is integer, if not error code -1
      std::cout << "Error: Time steps must be numeric." << "\n";
      return -1;}
    if (std::stoi(argv[1]) <= 0) { // Check if nts is negative
      std::cout << "Error: Invalid number of time steps." << "\n";
      return -2;}
    // Check if input file can be opened for reading
    std::ifstream infile(argv[2]);
    if (infile.is_open()) {
      infile.close();
    } else {
      std::cerr << "Error: Cannot open " << argv[2] << " for reading" << "\n";
      return -3;}
    // Check if output file can be opened for writing
    std::ifstream outfile(argv[3]);
    if (outfile.is_open()) {
      outfile.close();
    } else {
      std::cerr << "Error: Cannot open " << argv[3] << " for writing\n";
      return -4;}
    return 0;
}
