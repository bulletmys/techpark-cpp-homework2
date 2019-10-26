cmake_minimum_required(VERSION 2.6)
project(techpark)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(COVERAGE OFF CACHE BOOL "Coverage")

add_executable(techpark main.c logic_easy.h logic_easy.c logic_pro.c logic_pro.h input_helpers.c input_helpers.h)

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# adds the gtest include directory
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(gtest test.cpp logic_easy.h logic_pro.h logic_easy.c logic_pro.c)


list(APPEND CATCH_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
        )

ExternalProject_Add(
        catch
        GIT_REPOSITORY https://github.com/Bareflank/catch.git
        GIT_TAG v1.2
        GIT_SHALLOW 1
        CMAKE_ARGS ${CATCH_CMAKE_ARGS}
        PREFIX ${CMAKE_BINARY_DIR}/external/catch/prefix
        TMP_DIR ${CMAKE_BINARY_DIR}/external/catch/tmp
        STAMP_DIR ${CMAKE_BINARY_DIR}/external/catch/stamp
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/catch/download
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external/catch/src
        BINARY_DIR ${CMAKE_BINARY_DIR}/external/catch/build
)

# ------------------------------------------------------------------------------
# Hippomocks
# ------------------------------------------------------------------------------

list(APPEND HIPPOMOCKS_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
        )

ExternalProject_Add(
        hippomocks
        GIT_REPOSITORY https://github.com/Bareflank/hippomocks.git
        GIT_TAG v1.2
        GIT_SHALLOW 1
        CMAKE_ARGS ${HIPPOMOCKS_CMAKE_ARGS}
        PREFIX ${CMAKE_BINARY_DIR}/external/hippomocks/prefix
        TMP_DIR ${CMAKE_BINARY_DIR}/external/hippomocks/tmp
        STAMP_DIR ${CMAKE_BINARY_DIR}/external/hippomocks/stamp
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/hippomocks/download
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external/hippomocks/src
        BINARY_DIR ${CMAKE_BINARY_DIR}/external/hippomocks/build
)

# ------------------------------------------------------------------------------
# GSL
# ------------------------------------------------------------------------------

list(APPEND GSL_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
        )

ExternalProject_Add(
        gsl
        GIT_REPOSITORY https://github.com/Bareflank/GSL.git
        GIT_TAG v1.2
        GIT_SHALLOW 1
        CMAKE_ARGS ${GSL_CMAKE_ARGS}
        PREFIX ${CMAKE_BINARY_DIR}/external/gsl/prefix
        TMP_DIR ${CMAKE_BINARY_DIR}/external/gsl/tmp
        STAMP_DIR ${CMAKE_BINARY_DIR}/external/gsl/stamp
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/gsl/download
        SOURCE_DIR ${CMAKE_BINARY_DIR}/external/gsl/src
        BINARY_DIR ${CMAKE_BINARY_DIR}/external/gsl/build
)


# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if (ENABLE_CLANG_TIDY)

    find_program(CLANG_TIDY_BIN clang-tidy-4.0)
    find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-4.0.py)

    if (CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-4.0")
    endif ()

    if (RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate run-clang-tidy-4.0.py")
    endif ()

    list(APPEND RUN_CLANG_TIDY_BIN_ARGS
            -clang-tidy-binary ${CLANG_TIDY_BIN}
            -header-filter=.*
            -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
            )

    add_custom_target(
            tidy
            COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
            COMMENT "running clang tidy"
    )

endif ()

# ------------------------------------------------------------------------------
# CppCheck
# ------------------------------------------------------------------------------

if (ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY https://github.com/danmar/cppcheck.git
            GIT_TAG 1.79
            GIT_SHALLOW 1
            CMAKE_ARGS ${CPPCHECK_CMAKE_ARGS}
            PREFIX ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
            --enable=warning,style,performance,portability,unusedFunction
            --std=c11
            --verbose
            --error-exitcode=1
            --language=c
            -DMAIN=main
            -I ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/include/*.h
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/test/*.cpp
            )

    add_custom_target(
            check
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
            COMMENT "running cppcheck"
    )

endif ()


# ------------------------------------------------------------------------------
# Google Sanitizers
# ------------------------------------------------------------------------------

if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif ()

if (ENABLE_USAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif ()

if (ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif ()

# ------------------------------------------------------------------------------
# Valgrind
# ------------------------------------------------------------------------------

set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")

find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
INCLUDE(Dart)

# links gtest
target_link_libraries(gtest ${GTEST_LIBRARIES} pthread --coverage)
target_compile_options(gtest PRIVATE --coverage)