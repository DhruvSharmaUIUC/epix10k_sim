cmake_minimum_required(VERSION 3.16)
project(PixelSim)

# 1. Find Geant4 package with UI and Visualization drivers
find_package(Geant4 REQUIRED ui_all vis_all)

# 2. Setup Geant4 include directories and compiler flags
include(${Geant4_USE_FILE})

# 3. Tell CMake where to find your .hh files
include_directories(${PROJECT_SOURCE_DIR}/include)

# 4. Automatically locate all your .cc source files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# 5. Create the executable using the main file and all source files
add_executable(PixelSim PixelSim.cc ${sources})

# 6. Link the Geant4 libraries to your executable
target_link_libraries(PixelSim ${Geant4_LIBRARIES})

foreach(_script ${PIXEL_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()
