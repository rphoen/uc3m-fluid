# Add to this list all files related to sim library
add_library(sim
progargs.hpp
progargs.cpp
parser.hpp
parser.cpp
grid.cpp
grid.hpp
block.cpp
block.hpp
        simulation.hpp
        simulation.cpp
        constants.cpp
        constants.hpp
        particle.hpp
        particle.cpp
        formulas.hpp
        formulas.cpp
)
# Use this line only if you have dependencies from stim to GSL
target_link_libraries (sim PRIVATE Microsoft.GSL::GSL)
