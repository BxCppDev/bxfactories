message(STATUS "Welcome in BxFactories Example 1 !")

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(BxFactoriesExample1 VERSION 1.0)
message(STATUS "BxFactories_DIR          = '${BxFactories_DIR}'")
find_package(BxFactories REQUIRED CONFIG)
message(STATUS "BxFactories_VERSION      = '${BxFactories_VERSION}'")
message(STATUS "BxFactories_INCLUDE_DIRS = '${BxFactories_INCLUDE_DIRS}'")
include_directories(${BxFactories_INCLUDE_DIRS})
add_executable(example1 example1.cxx)

# - end