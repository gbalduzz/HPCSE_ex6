cmake_minimum_required(VERSION 3.1)
project(ex6)

set(ORDER 8 CACHE STRING "order of expansion")

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O4 -fstrict-aliasing -ffast-math -mtune=native -march=native -funroll-loops ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=3 -std=c++11  -Wstrict-aliasing=2 -fopenmp")

#compile ispc files
include(ispc.cmake)
include(m4generate.cmake)
set(M4_FLAGS -DORDER=${ORDER})
m4generate(src/p2e.ispc.m4)
m4generate(src/e2p.ispc.m4)
set(ISPC_SOURCES ${M4DIR}/p2e.ispc ${M4DIR}/e2p.ispc src/p2p.ispc)
create_ispc_library("${ISPC_SOURCES}")

#cuda part
find_package(CUDA)
set(CUDA_PROPAGATE_HOST_FLAGS OFF) 
#set(CUDA_VERBOSE_BUILD ON)
set(CUDA_NVCC_FLAGS -std=c++11)
cuda_include_directories(
  ${PROJECT_SOURCE_DIR}/include 
  ${M4DIR} 
  ${PROJECT_BINARY_DIR}/ispc/include
  )
m4generate(src/cuda_kernel.cuh.m4)
CUDA_COMPILE(CUDA_OBJ src/cuda_potential.cu)
CUDA_ADD_LIBRARY(cudaLib STATIC ${CUDA_OBJ})
target_link_libraries(cudaLib ispc)


add_executable(ex6 main.cpp src/particles.cpp)
add_definitions(-DORDER=${ORDER})
target_include_directories(ex6 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ex6 cudaLib ispc)




