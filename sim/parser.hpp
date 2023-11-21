#ifndef PARSER_H
#define PARSER_H

#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"
#include "simulation.hpp"
#include <fstream>
#include <iostream>
#include <locale>
#include <vector>

int parser(int argc, char **argv);

void mergeSort(std::vector<Particle> &particles, int l, int r);

#endif // PARSER_H
