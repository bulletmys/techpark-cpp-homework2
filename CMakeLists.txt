cmake_minimum_required(VERSION 2.6)
project(techpark)

set(CMAKE_CXX_STANDARD 14)
SET(COVERAGE OFF CACHE BOOL "Coverage")

add_executable(techpark main.c logic_easy.h logic_easy.c logic_pro.c logic_pro.h input_helpers.c input_helpers.h)

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# adds the gtest include directory
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(gtest test.cpp logic_easy.h logic_pro.h logic_easy.c logic_pro.c)

# links gtest
target_link_libraries(gtest ${GTEST_LIBRARIES} pthread --coverage)
target_compile_options(gtest PRIVATE --coverage)