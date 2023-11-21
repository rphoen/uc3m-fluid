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

int parser(char **argv);

// read binary value from file
Grid readInput(const std::string& inputfile);

Particle readParticle(std::ifstream &input_file, int count);

// print parameters
int printParameters(Grid &grid);

// write binary value to file
void writeOutput(const std::string& outputfile, Grid &grid);

void writeParticle(Particle particle, std::ofstream &output_file);

// mergesort for particles array
void merge(std::vector<Particle> &particles, int left, int middle, int right);

void mergeSort(std::vector<Particle> &particles, int left, int right);

#endif // PARSER_H
