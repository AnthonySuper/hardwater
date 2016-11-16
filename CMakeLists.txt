cmake_minimum_required(VERSION 2.8.0)

project(hardwater)

enable_testing()

set(PROJECT_SRCS
  src/mapped_file.cpp
  src/file_chunk.cpp)

set(PROJECT_HEADERS
  include/mapped_file.hpp
  include/file_chunk.hpp)


add_executable(app app.cpp
  ${PROJECT_SRCS} 
  ${PROJECT_HEADERS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(app ${OPENSSL_LIBRARIES})

include_directories(include/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
