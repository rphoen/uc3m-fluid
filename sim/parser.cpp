#include "parser.hpp"
#include "simulation.hpp"
#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"
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

int parser(char **argv) {
  int nts = std::stoi(argv[1]); // number of time steps
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
  auto ppm = read_binary_value<float>(input_file); // particles per meter
  int np = read_binary_value<int>(input_file);      // number of particles

  int count = -1; // count number of particles

  // Create Grid
  Grid grid(ppm, np);
  // update grid
  grid.update_grid();

  while (!input_file.eof()) {
    count += 1;
    // Read particle values
    std::vector<float> position;
    std::vector<float> hv;
    std::vector<float> velocity;

    position.push_back(read_binary_value<float>(input_file));
    position.push_back(read_binary_value<float>(input_file));
    position.push_back(read_binary_value<float>(input_file));
    hv.push_back(read_binary_value<float>(input_file));
    hv.push_back(read_binary_value<float>(input_file));
    hv.push_back(read_binary_value<float>(input_file));
    velocity.push_back(read_binary_value<float>(input_file));
    velocity.push_back(read_binary_value<float>(input_file));
    velocity.push_back(read_binary_value<float>(input_file));

    // Create particle
    Particle p(position, hv, velocity);
    grid.add_particle_to_block(p);
  }

  for (const auto& centerBlock : grid.get_blocks())
  {
    grid.findAdjBlocks(centerBlock.second);
  }

  if (count == np) {

    // output details of file
    std::cout << "Particles per meter: " << grid.get_ppm() << std::endl;
    std::cout << "np: " << grid.get_np() << std::endl;
    std::cout << "Smoothing length: " << grid.get_smoothingLength()
              << std::endl;
    std::cout << "Particle mass: " << grid.get_particleMass() << std::endl;
    std::cout << "Grid size: " << grid.get_numberX() << " x "
              << grid.get_numberY() << " x " << grid.get_numberZ() << std::endl;
    std::cout << "Number of blocks: " << grid.get_numBlocks() << std::endl;
    std::cout << "Block size: " << grid.get_sizeX() << " x " << grid.get_sizeY()
              << " x " << grid.get_sizeZ() << std::endl;

    for (int i = 0; i < nts; i++)
    {
      simulateOneStep(grid);
    }

  } else {
    std::cout << "Error: Number of particles mismatch. Header: " << np
              << ", Found: " << count << std::endl;
  }


  // Now, need to write all the new particle information into the output files
  write_binary_value(ppm, output_file);
  write_binary_value(np, output_file);

  for (auto particle : grid.get_particles())
  {
    float px = particle.get_px();
    float py = particle.get_py();
    float pz = particle.get_pz();
    float hvx = particle.get_hvx();
    float hvy = particle.get_hvy();
    float hvz = particle.get_hvz();
    float ax = particle.get_ax();
    float ay = particle.get_ay();
    float az = particle.get_az();

    write_binary_value(px, output_file);
    write_binary_value(py, output_file);
    write_binary_value(pz, output_file);
    write_binary_value(hvx, output_file);
    write_binary_value(hvy, output_file);
    write_binary_value(hvz, output_file);
    write_binary_value(ax, output_file);
    write_binary_value(ay, output_file);
    write_binary_value(az, output_file);
  }

  // Close files
  input_file.close();
  output_file.close();

  return 0;
}
