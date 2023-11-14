// #include "parser.hpp"
#include "block.hpp"
#include <fstream>
#include <iostream>
#include <locale>

using namespace std;

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
char *as_writable_buffer(T &value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<char *>(&value);
}

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
char const *as_buffer(T const &value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<char const *>(&value);
}

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
T read_binary_value(std::istream &is) {
  T value{};
  is.read(as_writable_buffer(value), sizeof(value));
  return value;
}

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
void write_binary_value(T value, std::ostream &os) {
  os.write(as_buffer(value), sizeof(value));
}

int parser(int argc, char **argv) {
  int nts = atoi(argv[1]); // number of time steps
  std::string inputfile = argv[2];
  std::string outputfile = argv[3];

  // print all arguments
  std::cout << "Number of time steps: " << nts << std::endl;
  std::cout << "inputfile: " << inputfile << std::endl;
  std::cout << "outputfile: " << outputfile << std::endl;
  std::cout << std::endl;

  // Open files
  std::ifstream input_file(inputfile, std::ios::binary);
  std::ofstream output_file(outputfile, std::ios::binary);

  // Read header vales
  float ppm = read_binary_value<float>(input_file); // particles per meter
  int np = read_binary_value<int>(input_file);      // number of particles

  int count = -1; // count number of particles

  // Initialize array of Particles
  std::vector<Particle> particles;

  while (!input_file.eof()) {
    count += 1;
    // Read particle values
    float px = read_binary_value<float>(input_file);
    float py = read_binary_value<float>(input_file);
    float pz = read_binary_value<float>(input_file);
    float hvx = read_binary_value<float>(input_file);
    float hvy = read_binary_value<float>(input_file);
    float hvz = read_binary_value<float>(input_file);
    float vx = read_binary_value<float>(input_file);
    float vy = read_binary_value<float>(input_file);
    float vz = read_binary_value<float>(input_file);

    particles.push_back(Particle(px, py, pz, hvx, hvy, hvz, vx, vy, vz));
  }

  if (count == np) {
    Constants::ppm = ppm;
    Constants::np = np;

    // update simulation parameters
    Constants::update_sim_params();

    // update box parameters
    Box::update_box_params();

    // output details of file
    // Particles per meter: 204
    // Smoothing length: 0.00830882
    // Particle mass: 0.00011779
    // Grid size: 15 x 21 x 15
    // Number of blocks: 4725
    // Block size: 0.00866667 x 0.00857143 x 0.00866667
    std::cout << "Particles per meter: " << Constants::ppm << std::endl;
    std::cout << "np: " << Constants::np << std::endl;
    std::cout << "Smoothing length: " << Constants::smoothingLength
              << std::endl;
    std::cout << "Particle mass: " << Constants::particleMass << std::endl;
    std::cout << "Grid size: " << Box::numberX << " x " << Box::numberY << " x "
              << Box::numberZ << std::endl;
    std::cout << "Number of blocks: " << Box::numBlocks << std::endl;
    std::cout << "Block size: " << Box::sizeX << " x " << Box::sizeY << " x "
              << Box::sizeZ << std::endl;

  } else {
    std::cout << "Error: Number of particles mismatch. Header: " << np
              << ", Found: " << count << std::endl;
  }

  // Close files
  input_file.close();
  output_file.close();

  return 0;
}
