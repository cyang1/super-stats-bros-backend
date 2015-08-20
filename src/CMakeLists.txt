cmake_minimum_required(VERSION 2.8)
project(superstatsbros)

include(build/CMakeLists.txt)

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

add_subdirectory(superstatsbros)
