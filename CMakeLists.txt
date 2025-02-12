cmake_minimum_required(VERSION 3.3)

if(APPLE)
   set(CMAKE_C_COMPILER /usr/local/Homebrew/bin/gcc-14)
   set(CMAKE_CXX_COMPILER /usr/local/Homebrew/bin/g++-14)
endif()

project(collision_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
   include_directories(SYSTEM /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
   include_directories(/usr/local/Homebrew/Cellar/gcc/14.2.0_1/include/c++/14)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(main collision.cpp collision_parser.cpp collision_manager.cpp main.cpp)

enable_testing()

add_executable(
  collion_manager_test
  collision.cpp
  collision_parser.cpp
  collision_manager.cpp
  collision_manager_test.cpp
)

target_link_libraries(
  collion_manager_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(collion_manager_test)
