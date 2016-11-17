cmake_minimum_required(VERSION 2.8.0)

project(hardwater)

enable_testing()

set(PROJECT_SRCS
  src/mapped_file.cpp
  src/fragment_hash.cpp
  src/ion.cpp
  src/chunk.cpp
  src/solution.cpp
  src/key.cpp)

set(PROJECT_HEADERS
  include/mapped_file.hpp
  include/fragment_hash.hpp
  include/ion.hpp
  include/chunk.hpp
  include/solution.hpp
  include/key.hpp)


add_executable(app app.cpp
  ${PROJECT_SRCS} 
  ${PROJECT_HEADERS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(app ${OPENSSL_LIBRARIES})




include_directories(include/)
include_directories(lib/)


set(PROJECT_TEST_SRCS
  test_srcs/ion_test.cpp
  test_srcs/key_test.cpp)

add_executable(catch_test test_srcs/main.cpp 
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS})

source_group("Test Files" FILES ${PROJECT_TEST_SRCS})

target_link_libraries(catch_test ${OPENSSL_LIBRARIES})

add_custom_command(TARGET catch_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files
  $<TARGET_FILE_DIR:catch_test>/test_files)

add_test(NAME CatchTests COMMAND catch_test)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
