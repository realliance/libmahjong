# libmahjong

`libmahjong` is a library for running games of Riichi Mahjong, originally built to power [Riichi Royale](https://github.com/realliance/RiichiRoyale), and later [Gametable](https://github.com/realliance/gametable). The library uses a state machine to represent the status of the game, and allows for players (whether artificial or real) to interact at appropriate times. 

# Development

## Getting Started

Install the following dependencies:

- `git`

- `cmake`

- `clang` or `gcc`

Optionally:

- `gtest` (For testing support)

- `ctcache` (For quicker compilation with clang-tidy enabled)

Clone the repository:

```
git clone git@github.com:realliance/libmahjong.git
```

Run the following commands within the project directory:
```
# Bootstrap CMake Environment
cmake -S . -B build

# CMake project opinions (default)
 * libmahjong_build_tools: whether or not examples should be built (ON)
 * libmahjong_build_tests: whether or not tests should be built (ON)
 * libmahjong_use_clang_utils: whether or not project is compiled with clang-tidy (ON)

# Build Project
cmake --build build
```

## Testing and Documentation

The test suite is located in `tests/` and can be run with `ctest --test-dir build --output-on-failure`
