cmake_minimum_required(VERSION 3.3)

if(APPLE)
   set(CMAKE_C_COMPILER /usr/local/Homebrew/bin/gcc-14)
   set(CMAKE_CXX_COMPILER /usr/local/Homebrew/bin/g++-14)
endif()

project(collision_manager)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "Found OpenMP ${OpenMP_CXX_VERSION}")
else()
    message(STATUS "OpenMP not found!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
   include_directories(SYSTEM /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
   include_directories(/usr/local/Homebrew/Cellar/gcc/14.2.0_1/include/c++/14)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O3")

# Enable Link-Time Optimizations for Clang only
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    cmake_policy(SET CMP0069 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)

    if(ipo_supported)
        set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto=thin")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=thin")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto=thin")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto=thin")
    endif()
endif()

option(ENABLE_PGO "Enable Profile Guided Optimization" OFF)
option(GENERATE_PGO_PROFILE "Mode to build binary that generates profile files" OFF)

if(ENABLE_PGO)
    if(GENERATE_PGO_PROFILE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GCC")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-generate")
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate")
        endif()
    else()
        if(CMAKE_CXX_COMPILER_ID MATCHES "GCC")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use")
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-use=${CMAKE_BINARY_DIR}/default.profdata -Wno-profile-instr-out-of-date")
        endif()
    endif()
endif()

if(OpenMP_CXX_FOUND)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.1
)

if(APPLE)
set (HAVE_STD_REGEX ON)
set (RUN_HAVE_STD_REGEX 1)
endif()

FetchContent_MakeAvailable(benchmark)

add_library(collision_manager query.cpp collision.cpp collision_parser.cpp collision_manager.cpp)
target_link_libraries(collision_manager PUBLIC OpenMP::OpenMP_CXX)

add_executable(main main.cpp)
target_link_libraries(main collision_manager)

add_executable(
  collision_manager_test
  collision_manager_test.cpp
)

target_link_libraries(
  collision_manager_test
  collision_manager
  GTest::gtest_main
)

add_executable(
  collision_manager_benchmark
  collision_manager_benchmark.cpp
)
target_link_libraries(
  collision_manager_benchmark
  collision_manager
  benchmark::benchmark
)

add_executable(
  collision_parser_benchmark
  collision_parser_benchmark.cpp
)
target_link_libraries(
  collision_parser_benchmark
  collision_manager
  benchmark::benchmark_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(collision_manager_test)
