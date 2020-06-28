# - Top level CMakeLists.txt for BxFactories C++ library
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "${PROJECT_NAME} requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(BxFactories VERSION 1.0)
# - Version of the original fortran BxFactories
set(BxFactories_TAG "BxFactories-${BxFactories_VERSION}")
message(STATUS "BxFactories_TAG=${BxFactories_TAG}")

# - Standard Modules
# For "BUILD_TESTING" option
include(CTest)

# Install directories
include(GNUInstallDirs)

# Require relative paths to support relocation
foreach(_dir BINDIR LIBDIR INCLUDEDIR DATAROOTDIR)
  if(IS_ABSOLUTE "${CMAKE_INSTALL_${_dir}}")
    message(FATAL_ERROR "Absolute path for CMAKE_INSTALL_${_dir} not allowed")
  endif()
endforeach()


# Require 
find_package(Boost 1.65 REQUIRED COMPONENTS program_options)
message( STATUS "Boost version             : ${Boost_VERSION}")
message( STATUS "Boost version             : ${Boost_LIB_VERSION}")
message( STATUS "Boost include directories : ${Boost_INCLUDE_DIRS}")
## message( STATUS "Boost libraries : ${Boost_LIBRARIES}")
include_directories(Boost_INCLUDE_DIRS)

# Add a path for CMake config files
set(CMAKE_INSTALL_CMAKEDIR      ${CMAKE_INSTALL_LIBDIR}/cmake)
set(CMAKE_INSTALL_FULL_CMAKEDIR ${CMAKE_INSTALL_FULL_LIBDIR}/cmake)

# - Publish headers
configure_file(source/bxfactories/version.hpp.in
  ${PROJECT_BINARY_DIR}/bxfactories/version.hpp @ONLY)

set(BxFactories_HEADERS
  ${PROJECT_BINARY_DIR}/bxfactories/version.hpp
  source/bxfactories/factory.hpp
  source/bxfactories/factory-inl.hpp
  source/bxfactories/factory_macros.hpp
  source/bxfactories/bxfactories.hpp
  )

# set(BxFactories_SOURCES
# )

# For now, use CMAKE_CXX_STANDARD to apply flag.
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/src)

# add_library(bxfactories SHARED ${BxFactories_HEADERS} ${BxFactories_SOURCES})
# target_compile_definitions(bxfactories PRIVATE ENABLE_BINRELOC)
# target_include_directories(bxfactories PUBLIC
#   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
#   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
#   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# )
# set_target_properties(bxfactories PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

install(FILES ${BxFactories_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bxfactories
  )

# install(TARGETS bxfactories
#   EXPORT BxFactoriesTargets
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   )

install(FILES ${PROJECT_SOURCE_DIR}/LICENSE.txt
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${BxFactories_TAG}
  )

if(BUILD_TESTING)
  set(BxFactories_TESTS
    # testing/test-utils.cxx
   )
  # set(_bxfactories_TEST_ENVIRONMENT "BXFACTORIES_RESOURCE_DIR=${PROJECT_SOURCE_DIR}/resources")
  
  foreach(_testsource ${BxFactories_TESTS})
    get_filename_component(_testname "${_testsource}" NAME_WE)
    set(_testname "bxfactories-${_testname}")
    add_executable(${_testname} ${_testsource})
    target_link_libraries(${_testname} bxfactories)
    add_test(NAME ${_testname} COMMAND ${_testname})
    # set_property(TEST ${_testname}
    #   APPEND PROPERTY ENVIRONMENT ${_bxfactories_TEST_ENVIRONMENT}
    #   )
  endforeach()
endif()

#-----------------------------------------------------------------------
# Configure/Install support files
# bxfactories-config program
configure_file(bxfactories-config.in
  ${PROJECT_BINARY_DIR}/bxfactories-config
  @ONLY)

install(FILES
  ${PROJECT_BINARY_DIR}/bxfactories-config
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  PERMISSIONS
  OWNER_READ OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
  )

# - PkgConfig
configure_file(cmake/bxfactories.pc.in
  "${PROJECT_BINARY_DIR}/cmake/bxfactories.pc"
  @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/cmake/bxfactories.pc"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/
  )

# - CMake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
   "${PROJECT_BINARY_DIR}/BxFactoriesConfigVersion.cmake"
   VERSION ${BxFactories_VERSION}
   COMPATIBILITY SameMajorVersion
   )

# Build tree
# targets...
# export(EXPORT BxFactoriesTargets
#   NAMESPACE BxFactories
#   FILE "${PROJECT_BINARY_DIR}/BxFactoriesTargets.cmake"
#   )

# config file...
set(BxFactories_CONFIG_INCLUDEDIR "${PROJECT_SOURCE_DIR}")
configure_package_config_file(cmake/BxFactoriesConfig.cmake.in
  "${PROJECT_BINARY_DIR}/BxFactoriesConfig.cmake"
  INSTALL_DESTINATION "."
  INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
  PATH_VARS BxFactories_CONFIG_INCLUDEDIR
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

# Install Tree
# targets...
# install(EXPORT BxFactoriesTargets
#   NAMESPACE 
#   DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/BxFactories
#   )

set(BxFactories_CONFIG_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
configure_package_config_file(cmake/BxFactoriesConfig.cmake.in
  "${PROJECT_BINARY_DIR}/cmake/BxFactoriesConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_CMAKEDIR}/BxFactories"
  PATH_VARS BxFactories_CONFIG_INCLUDEDIR
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

install(FILES
  ${PROJECT_BINARY_DIR}/BxFactoriesConfigVersion.cmake
  ${PROJECT_BINARY_DIR}/cmake/BxFactoriesConfig.cmake
  DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/BxFactories
  )

# install(DIRECTORY "${PROJECT_SOURCE_DIR}/resources/"
#   DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${BxFactories_TAG}/resources
#   FILE_PERMISSIONS
#   OWNER_READ 
#   GROUP_READ 
#   WORLD_READ 
#   DIRECTORY_PERMISSIONS
#   OWNER_READ OWNER_EXECUTE
#   GROUP_READ GROUP_EXECUTE
#   WORLD_READ WORLD_EXECUTE
#   )

install(DIRECTORY "${PROJECT_SOURCE_DIR}/examples/"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${BxFactories_TAG}/examples
  FILE_PERMISSIONS
  OWNER_READ 
  GROUP_READ 
  WORLD_READ 
  DIRECTORY_PERMISSIONS
  OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
  )

# - end
