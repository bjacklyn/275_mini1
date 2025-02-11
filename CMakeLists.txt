cmake_minimum_required(VERSION 3.3)
project(collision_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# set(CMAKE_C_COMPILER /usr/local/Homebrew/bin/gcc-14)
# set(CMAKE_CXX_COMPILER /usr/local/Homebrew/bin/g++-14)

# include_directories(SYSTEM /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
# include_directories(/usr/local/Homebrew/Cellar/gcc/14.2.0_1/include/c++/14)

if(APPLE)
   set(CMAKE_C_COMPILER /usr/local/Homebrew/bin/gcc-14)
   set(CMAKE_CXX_COMPILER /usr/local/Homebrew/bin/g++-14)

   include_directories(SYSTEM /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
   include_directories(/usr/local/Homebrew/Cellar/gcc/14.2.0_1/include/c++/14)

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
elseif(WIN32)
   set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
   set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")
   include_directories("C:/mingw64/include")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")


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
