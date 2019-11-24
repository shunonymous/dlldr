cmake_minimum_required(VERSION 3.2)
project(dlldr CXX)

#add_definitions("-DUSE_EXPERIMENTAL_FILESYSTEM")

# PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

### C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warning
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra --pedantic-errors")
endif()

include_directories(include/)
add_subdirectory(src/)

if(NOT DLLDR_NO_BUILD_EXAMPLE)
  add_subdirectory(example/)
endif()
