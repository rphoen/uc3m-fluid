#include "sim/progargs.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
  // parsing arguments
  progargs(argc, argv);

  // success
  int nts = atoi(argv[1]);
  std::string inputfile = argv[2];
  std::string outputfile = argv[3];

  // print all arguments
  std::cout << "nts: " << nts << std::endl;
  std::cout << "inputfile: " << inputfile << std::endl;
  std::cout << "outputfile: " << outputfile << std::endl;

  // parsing input file
  std::ifstream input_file(inputfile, std::ios::binary);
  // print first line of file
  std::string line;
  std::getline(input_file, line);
  std::cout << line << std::endl;

  // output file
  std::ofstream output_file(outputfile, std::ios::binary);

  return 0;
}
