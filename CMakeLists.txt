cmake_minimum_required(VERSION 3.10)

project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_library(Render src/Render.cpp src/Image.cpp src/Config.cpp)

add_executable(RayTracing src/main.cpp)

target_link_libraries(RayTracing PUBLIC Render)

find_package(Threads REQUIRED)
target_link_libraries(Render ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(Render PUBLIC
                                      "${PROJECT_SOURCE_DIR}/include"
                                      )

target_include_directories(RayTracing PUBLIC
                                      "${PROJECT_SOURCE_DIR}/include"
                                      )
