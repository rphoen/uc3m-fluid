#include "../sim/parser.hpp"
#include "../sim/progargs.hpp"

#include <iostream>
#include <span>

int main(int argc, char **argv) {
  // arguments
  std::array<char *, 4> args = {argv[0], argv[1], argv[2], argv[3]};
  if (progargs(argc, args) == 0) {
    parser(args);
  }

  return 0;
}
