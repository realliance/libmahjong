# libmahjong

`libmahjong` is a library for running games of Riichi Mahjong, originally built to power [Riichi Royale](https://github.com/realliance/RiichiRoyale), and later [Gametable](https://github.com/realliance/gametable). The library uses a state machine to represent the status of the game, and allows for players (whether artificial or real) to interact at appropriate times. 

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

## Attribution

The Riichi Mahjong 2016 Ruleset is redistributed from the [European Mahjong Association](http://mahjong-europe.org/portal/images/docs/Riichi-rules-2016-EN.pdf) under the CC BY-NC-SA 4.0. Their ruleset forms the basis for libmahjong.
