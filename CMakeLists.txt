cmake_minimum_required(VERSION 3.10)

project(RayTracing)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_library(Render src/Render.cpp src/Image.cpp src/Config.cpp)

add_executable(RayTracing src/main.cpp)

target_link_libraries(RayTracing PUBLIC Render)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
   target_link_libraries(RayTracing PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(Render PUBLIC
                                      "${PROJECT_SOURCE_DIR}/include"
                                      )

target_include_directories(RayTracing PUBLIC
                                      "${PROJECT_SOURCE_DIR}/include"
                                      )
