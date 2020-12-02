# libmahjong

# Development

## Getting Started

### On Linux

Install the following dependencies:

- `git`

- `cmake`

- `clang` (or [clang-11](https://packages.ubuntu.com/bionic/clang-11))

### On Windows

Windows requires Visual Studio 2019, which can be found [here](https://visualstudio.microsoft.com/downloads/), and CMake, which can be found [here](https://cmake.org/download/)

Clone the repository:

```
git clone git@github.com:realliance/libmahjong.git
```

Run the following commands within the project directory (note that these commands are for ):
```
# Bootstrap CMake Environment
cmake -S . -B build

# Or bootstrap CMake with tool building enabled
cmake -S . -B build -DPACKAGE_TOOLS=ON

# Or, if wanting to use Clang (Linux Only)

C=clang CXX=clang++ cmake -S . -B build

# Build Project
cmake --build build
```

## Testing and Documentation

The test suite is located in `tests/` and can be run with `make -C build check`
