cmake_minimum_required(VERSION 3.3)
project(collision_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

add_executable(main collision.cpp collision_parser.cpp collision_manager.cpp main.cpp)
