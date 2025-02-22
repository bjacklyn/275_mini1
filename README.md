# Build and Execution Instructions

## 1. Extract collisions csv file to top-level of git repo
```
tar -xvf <data.tar.gz>
```

## 2. Build code
```
cd phase_<X>
mkdir build
cd build

# Optionally set the compiler with env vars or lmod
export CC=<your c compiler>
export CXX=<your c++ compiler>

# Use cmake to build project
#
# Except phase 3.9 which adds the -DENABLE_PGO=ON -DGENERATE_PGO_PROFILE={ON,OFF}
# cmake flags to build with profile guided optimizations
cmake ..
make
```

## 3. Use ctest to execute unit tests
```
ctest
```

## 4. Execute benchmarks
```
./collision_manager_benchmarks
```
