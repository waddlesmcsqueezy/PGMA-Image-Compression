# CMakeList.txt : CMake project for IanHarmon-CS4378-Assign01, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (IanHarmon-CS4378-Assign01 "IanHarmon-CS4378-Assign01.cpp" "IanHarmon-CS4378-Assign01.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET IanHarmon-CS4378-Assign01 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
