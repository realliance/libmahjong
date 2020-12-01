# Riichi Royale
*Team "The One Cow" for Fall 2020 Performant Programming in Python*

## For TAs

### Project Description
Riichi Royale is a singleplayer game where you play 4-player Riichi Mahjong against AI opponents. The project includes a tutorial so you can learn how to play and (currently) a demo project in order to showcase our C++ Mahjong Game Manager

### Python Requirement
Riichi Royale has been verified to run with 64bit Python 3.8.x. We have verified against the Python 3.8.6 Windows x86-64 executable installer default installation, which can be found [here](https://www.python.org/downloads/release/python-386/).

If you encounter the error `ImportError: DLL load failed: %1 is not a valid Win32 application.`, this is due to having 32bit python.

If you encounter the error `ImportError: DLL load failed while importing libmahjong: The specified module could not be found.`, this is due to not using Python 3.8.

### Install
``pip install riichiroyale``

### To run Riichi Royale in your command prompt
``riichiroyale``

Depending on your installation of python, you may need to instead run 

```python -m riichiroyale```

# Development

## Getting Started

### On Linux

Install the following dependencies:

- `git`

- `cmake`

- `clang` (or [clang-10](https://packages.ubuntu.com/bionic/clang-10) if on Ubuntu < 20.04LTS)

### On Windows

Windows requires Visual Studio 2019, which can be found [here](https://visualstudio.microsoft.com/downloads/), and CMake, which can be found [here](https://cmake.org/download/)

Clone the repository:

```
git clone git@github.com:HartleyAHartley/LibMahjong.git
```

Run the following commands within the project directory (note that these commands are for ):
```
# Install python dependencies locally
pip install --user -r requirements.txt

# Create build directory for cmake
mkdir build/

# Bootstrap CMake Environment
cmake -S . -B build

# Or bootstrap CMake with tool building enabled
cmake -S . -B build -DPACKAGE_TOOLS=ON

# Or, if wanting to use Clang (Linux Only)

C=clang CXX=clang++ cmake -S . -B build

# Build Project
cmake --build build

# Bootstrap Development Environment
python setup.py develop --user

# Run Project
python riichiroyale
```

## Testing and Documentation

The python test suite is located in `test/` and can be run with `python test`

The C++ test suite is located in `cxxtests/` and can be run with `make -C build check`

## Good Resources

[DocDevs Mirror of Pygame Documentation](https://devdocs.io/pygame/): Standard Pygame API Docs. Pygame has a ratelimiter on their website of 50 visits per hour, which is very annoying while developing.

[Pygame Example Projects](https://github.com/ternus/pygame-examples): Great showing of how everything in pygame pieces together.

[Pybind11 Docs](https://pybind11.readthedocs.io/en/latest/): The documentation for pybind11, which is what we use for building the C++ python module.

## Attribution

### Music

lobby.ogg:
Airport Lounge by Kevin MacLeod
Link: https://incompetech.filmmusic.io/song/3347-airport-lounge
License: http://creativecommons.org/licenses/by/4.0/

game1.ogg:
Acid Trumpet by Kevin MacLeod
Link: https://incompetech.filmmusic.io/song/3340-acid-trumpet
License: http://creativecommons.org/licenses/by/4.0/

game2.ogg:
Shades of Spring by Kevin MacLeod
Link: https://incompetech.filmmusic.io/song/4342-shades-of-spring
License: http://creativecommons.org/licenses/by/4.0/

game3.ogg:
Backbay Lounge by Kevin MacLeod
Link: https://incompetech.filmmusic.io/song/3408-backbay-lounge
License: http://creativecommons.org/licenses/by/4.0/

### Icons

Icons are by Daniela Travieso, used with permission

### Trophy

[Trophy Icon](https://game-icons.net/1x1/lorc/trophy.html) by Lorc and used under CC BY 3.0