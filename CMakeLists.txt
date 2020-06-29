# (1)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(DARKSECTORG4)

# Find ROOT build modules
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net EG Minuit)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include( ${ROOT_USE_FILE} )

# Find GEANT4 build modules
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CONFIGURATION_TYPES Release;Debug;MinSizeRel;RelWithDebInfo)
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.hh)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(darksectorsim DarkSectorSim.cc ${sources} ${headers})
target_link_libraries(darksectorsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

install(TARGETS darksectorsim DESTINATION bin)

