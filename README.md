# The game-engine project

This is a Game Engine being built using C++, SDL3 and ImGUI.

## Mentors:

- [Adhesh Athrey](https://github.com/DedLad)
- [Anirudh Sudhir](https://github.com/anirudhsudhir)
- [Aditya Hegde](https://github.com/bwaklog)

## Mentees:

- [Andey Hemanth](https://github.com/Andy34G7)
- [Vinaayak G Dasika](https://github.com/Delta18-Git)
- [Selvaganesh Arunmozhi](https://github.com/thisisselva18)

## Dependencies

- [CMake](https://cmake.org/)
- [SDL3](https://www.libsdl.org/download-3.0.php)
- [ImGUI](https://github.com/ocornut/imgui)

## Building the project

```bash
# Clone the repository
git clone https://github.com/acmpesuecc/game-engine

# Create a build directory
mkdir build
cd build

# Run CMake to configure the project
cmake ..

# Build the project
cmake --build .
```

If using Visual Studio,

```bash
# Clone the repository
git clone https://github.com/acmpesuecc/game-engine

# Create a build directory
mkdir build
cd build

# Run CMake to configure the project
cmake .. -G "Visual Studio 17 2022" -A x64 #depending on your visual studio version

# Build the project
cmake --build . --config Release
```

## Running the project

```bash
./bin/game-engine
```

## Development Guidelines

- Running clang-format with diffs

```sh
chmod +x ci/linter.sh
./ci/linter.sh clangformat
```

- Running clang-tidy

```sh
./ci/linter.sh clangtidy
```
