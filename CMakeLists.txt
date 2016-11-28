cmake_minimum_required(VERSION 2.8.0)

project(hardwater)

enable_testing()

set(PROJECT_SRCS
  src/mapped_file.cpp
  src/fragment_hash.cpp
  src/ion.cpp
  src/chunk.cpp
  src/solution.cpp
  src/key.cpp
  src/ion_key.cpp
  src/util.cpp
  src/encrypted_ion_key.cpp
  src/sqlite3.c)

set(PROJECT_HEADERS
  include/mapped_file.hpp
  include/fragment_hash.hpp
  include/ion.hpp
  include/chunk.hpp
  include/solution.hpp
  include/key.hpp
  include/ion_key.hpp
  include/util.hpp
  include/encrypted_ion_key.hpp
  include/sqlite3.h)

add_executable(app app.cpp
  ${PROJECT_SRCS} 
  ${PROJECT_HEADERS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(app ${OPENSSL_LIBRARIES})

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

include_directories(include/)

set(PROJECT_TEST_SRCS
  test_srcs/ion_test.cpp
  test_srcs/key_test.cpp
  test_srcs/ion_key_test.cpp
  test_srcs/encrypted_ion_key_test.cpp
  test_srcs/key_helpers.cpp)

set(PROJECT_TEST_HEADERS
  test_include/catch.hpp
  test_include/key_helpers.hpp)

add_executable(catch_test test_srcs/main.cpp 
  ${PROJECT_SRCS}
  ${PROJECT_TEST_SRCS}
  ${PROJECT_TEST_HEADERS})

target_include_directories(catch_test PRIVATE test_include/)

source_group("Test Files" FILES ${PROJECT_TEST_SRCS})

target_link_libraries(catch_test ${OPENSSL_LIBRARIES})

add_custom_command(TARGET catch_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/test_files
  $<TARGET_FILE_DIR:catch_test>/test_files)

add_test(NAME CatchTests COMMAND catch_test)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
