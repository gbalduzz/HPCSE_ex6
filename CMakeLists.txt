cmake_minimum_required(VERSION 3.1)
project(ex6)

#set(CUDA_HOST_COMPILER "/usr/bin/gcc-4.9" CACHE STRING "cuda CC compiler" FORCE)
#set(CUDA_GPU_ARCH "-arcg=compute_50" CACHE STRING "gpu architecture" FORCE)
find_package(CUDA)

set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O4 -fstrict-aliasing -ffast-math -mtune=native -march=native -funroll-loops ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=3 -std=c++14  -Wstrict-aliasing=2 -fopenmp")

#compile ispc files
include(ispc.cmake)
include(m4generate.cmake)
m4generate(src/p2e.ispc.m4)
m4generate(src/e2p.ispc.m4)
set(ISPC_SOURCES ${M4DIR}/p2e.ispc ${M4DIR}/e2p.ispc src/p2p.ispc)
create_ispc_library("${ISPC_SOURCES}")

include_directories(ex6 PRIVATE ${PROJECT_SOURCE_DIR}/include)
add_executable(ex6 main.cpp)
target_link_libraries(ex6 ispc)




