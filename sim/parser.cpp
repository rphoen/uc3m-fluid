#include "parser.hpp"

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

  // Read input file
  Grid grid = readInput(inputfile);

  // Print parameters and simulation
  if (printParameters(grid) == 1) {
    // simulation here
    for (int i = 0; i < nts; i++) {
      simulateOneStep(grid);
    }
  }

  // Write output file
  writeOutput(outputfile, grid);

  return 0;
}

// read input file
Grid readInput(std::string inputfile) {
  std::ifstream input_file(inputfile, std::ios::binary);

  // Read header vales
  auto ppm = read_binary_value<float>(input_file); // particles per meter
  int np = read_binary_value<int>(input_file);

  // Create and update Grid
  Grid grid(ppm, np);
  grid.update_grid();

  int count = -1; // count number of particles

  while (!input_file.eof()) {
    count += 1;
    grid.add_particle_to_block(readParticle(input_file, count));
  }
  input_file.close();

  grid.set_count(count);
  for (const auto &centerBlock : grid.get_blocks()) {
    grid.findAdjBlocks(centerBlock.second);
  }

  return grid;
}

Particle readParticle(std::ifstream &input_file, int count) {
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
  Particle p(count, position, hv, velocity);

  return p;
}

// print parameters
int printParameters(Grid &grid) {
  if (grid.get_count() == grid.get_np()) {
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
    return 1;
  } else {
    std::cout << "Error: Number of particles mismatch. Header: "
              << grid.get_np() << ", Found: " << grid.get_count() << std::endl;
    return 0;
  }
}

void writeOutput(std::string outputfile, Grid &grid) {
  std::ofstream output_file(outputfile, std::ios::binary);

  // Sort all the particles
  std::vector<Particle> particles;
  for (auto block : grid.get_blocks()) {
    std::vector<Particle> temp = block.second.getParticles();
    particles.insert(particles.end(), temp.begin(), temp.end());
  }
  mergeSort(particles, 0, particles.size() - 1);

  write_binary_value(grid.get_ppm(), output_file);
  write_binary_value(grid.get_np(), output_file);

  for (auto particle : particles) {
    writeParticle(particle, output_file);
  }
  output_file.close();
}

void writeParticle(Particle particle, std::ofstream &output_file) {
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

// Merge sort ascending order by particle.get_id()
void merge(std::vector<Particle> &particles, int left, int middle, int right) {
  int n1 = middle - left + 1;
  int n2 = right - middle;

  std::vector<Particle> L(particles.begin() + left,
                          particles.begin() + left + n1);
  std::vector<Particle> R(particles.begin() + middle + 1,
                          particles.begin() + middle + 1 + n2);

  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    particles[k++] = (L[i].get_id() <= R[j].get_id()) ? L[i++] : R[j++];
  }

  while (i < n1) {
    particles[k++] = L[i++];
  }

  while (j < n2) {
    particles[k++] = R[j++];
  }
}

// void merge(std::vector<Particle> &particles, int left, int middle, int right)
// {
//   int i, j, k;
//   int n1 = middle - left + 1;
//   int n2 = right - middle;
//
//   /* create temp arrays */
//   std::vector<Particle> L;
//   std::vector<Particle> R;
//
//   /* Copy data to temp arrays L[] and R[] */
//   for (i = 0; i < n1; i++)
//     L.emplace_back(particles[left + i]);
//   for (j = 0; j < n2; j++)
//     R.emplace_back(particles[middle + 1 + j]);
//
//   /* Merge the temp arrays back into arr[l..r]*/
//   i = 0;    // Initial index of first subarray
//   j = 0;    // Initial index of second subarray
//   k = left; // Initial index of merged subarray
//   while (i < n1 && j < n2) {
//     if (L[i].get_id() <= R[j].get_id()) {
//       particles[k] = L[i];
//       i++;
//     } else {
//       particles[k] = R[j];
//       j++;
//     }
//     k++;
//   }
//
//   /* Copy the remaining elements of L[], if there are any */
//   while (i < n1) {
//     particles[k] = L[i];
//     i++;
//     k++;
//   }
//
//   /* Copy the remaining elements of R[], if there are any */
//   while (j < n2) {
//     particles[k] = R[j];
//     j++;
//     k++;
//   }
//
//   L.clear();
//   R.clear();
// }

void mergeSort(std::vector<Particle> &particles, int left, int right) {
  if (left < right) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int middle = left + (right - left) / 2;

    // Sort first and second halves
    mergeSort(particles, left, middle);
    mergeSort(particles, middle + 1, right);

    merge(particles, left, middle, right);
  }
}
