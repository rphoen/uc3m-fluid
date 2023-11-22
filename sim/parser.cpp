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

int parser(std::array<char *, 4> args) {
  int const nts = std::stoi(args[1]); // number of time steps
  std::string const inputfile = args[2];
  std::string const outputfile = args[3];

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
Grid readInput(const std::string &inputfile) {
  std::ifstream input_file(inputfile, std::ios::binary);

  // Read header vales
  auto ppm = read_binary_value<float>(input_file); // particles per meter
  int const nump = read_binary_value<int>(input_file);

  // Create and update Grid
  Grid grid(ppm, nump);
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
  std::vector<float> vectorhv;
  std::vector<float> velocity;

  position.push_back(read_binary_value<float>(input_file));
  position.push_back(read_binary_value<float>(input_file));
  position.push_back(read_binary_value<float>(input_file));
  vectorhv.push_back(read_binary_value<float>(input_file));
  vectorhv.push_back(read_binary_value<float>(input_file));
  vectorhv.push_back(read_binary_value<float>(input_file));
  velocity.push_back(read_binary_value<float>(input_file));
  velocity.push_back(read_binary_value<float>(input_file));
  velocity.push_back(read_binary_value<float>(input_file));

  // Create particle
  Particle const part(count, position, vectorhv, velocity);

  return part;
}

// print parameters
int printParameters(Grid &grid) {
  if (grid.get_count() == grid.get_np()) {
    std::cout << "Particles per meter: " << grid.get_ppm() << '\n';
    std::cout << "np: " << grid.get_np() << '\n';
    std::cout << "Smoothing length: " << grid.get_smoothingLength() << '\n';
    std::cout << "Particle mass: " << grid.get_particleMass() << '\n';
    std::cout << "Grid size: " << grid.get_numberX() << " x "
              << grid.get_numberY() << " x " << grid.get_numberZ() << '\n';
    std::cout << "Number of blocks: "
              << grid.get_numberX() * grid.get_numberY() * grid.get_numberZ()
              << '\n';
    std::cout << "Block size: " << grid.get_sizeX() << " x " << grid.get_sizeY()
              << " x " << grid.get_sizeZ() << '\n';
    return 1;
  }
  std::cout << "Error: Number of particles mismatch. Header: " << grid.get_np()
            << ", Found: " << grid.get_count() << '\n';
  return 0;
}

void writeOutput(const std::string &outputfile, Grid &grid) {
  std::ofstream output_file(outputfile, std::ios::binary);

  // Sort all the particles
  std::vector<Particle> particles;
  for (auto block : grid.get_blocks()) {
    std::vector<Particle> temp = block.second.getParticles();
    particles.insert(particles.end(), temp.begin(), temp.end());
  }
  mergeSort(particles, 0, static_cast<int>(particles.size() - 1));

  write_binary_value(grid.get_ppm(), output_file);
  write_binary_value(grid.get_np(), output_file);

  for (const auto &particle : particles) {
    writeParticle(particle, output_file);
  }
  output_file.close();
}

void writeParticle(Particle particle, std::ofstream &output_file) {
  float const posx = particle.get_px();
  float const posy = particle.get_py();
  float const posz = particle.get_pz();
  float const hvx = particle.get_hvx();
  float const hvy = particle.get_hvy();
  float const hvz = particle.get_hvz();
  double const accx = particle.get_ax();
  double const accy = particle.get_ay();
  double const accz = particle.get_az();

  write_binary_value(posx, output_file);
  write_binary_value(posy, output_file);
  write_binary_value(posz, output_file);
  write_binary_value(hvx, output_file);
  write_binary_value(hvy, output_file);
  write_binary_value(hvz, output_file);
  write_binary_value(accx, output_file);
  write_binary_value(accy, output_file);
  write_binary_value(accz, output_file);
}

// Merge sort ascending order by particle.get_id()
void merge(std::vector<Particle> &particles, int left, int middle, int right) {
  int const num1 = middle - left + 1;
  int const num2 = right - middle;

  std::vector<Particle> lll(particles.begin() + left,
                            particles.begin() + left + num1);
  std::vector<Particle> rrr(particles.begin() + middle + 1,
                            particles.begin() + middle + 1 + num2);

  int iii = 0;
  int jjj = 0;
  int kkk = left;

  while (iii < num1 && jjj < num2) {
    particles[kkk++] =
        (lll[iii].get_id() <= rrr[jjj].get_id()) ? lll[iii++] : rrr[jjj++];
  }

  while (iii < num1) {
    particles[kkk++] = lll[iii++];
  }

  while (jjj < num2) {
    particles[kkk++] = rrr[jjj++];
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

// mergesort recursive (don't delete)
// void mergeSort(std::vector<Particle> &particles, int left, int right) {
//   if (left < right) {
//     // Same as (l+r)/2, but avoids overflow for
//     // large l and h
//     int const middle = left + (right - left) / 2;
//
//     // Sort first and second halves
//     mergeSort(particles, left, middle);
//     mergeSort(particles, middle + 1, right);
//
//     merge(particles, left, middle, right);
//   }
// }

// mergesort interative to fix clang-tidy warning
#include <vector>

void mergeSort(std::vector<Particle> &particles, int left, int right) {
  for (int currentSize = 1; currentSize <= right - left; currentSize *= 2) {
    for (int leftStart = left; leftStart < right;
         leftStart += 2 * currentSize) {
      int const middle = std::min(leftStart + currentSize - 1, right);
      int const rightEnd = std::min(leftStart + 2 * currentSize - 1, right);
      merge(particles, leftStart, middle, rightEnd);
    }
  }
}
