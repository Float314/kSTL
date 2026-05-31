# kSTL
A freestanding and minimal implementation of the C++ Standard Template Library (STL)
This repository also includes a minimal C++ Runtime for initialization of constructors and a main function

Before we get started, an important note: **This STL implementation does not aim to be 100% standards-compliant, instead it aims to be a simple easy-to-use freestanding version of the STL.**

# Building (for non-freestanding)
## Linux / macOS
Steps:
- Clone the repo: `git clone https://github.com/xNoerPlaysCodes/kSTL`
- Run CMake with default flags: `cmake -S . -B build`
- Build: `cmake --build build`

That's it! \
You can run the tests via: \
`bin/tester` \
but make sure you are in the root of the project directory

# Building (for freestanding)
## Linux / macOS
Steps:
- Clone the repo: `git clone https://github.com/xNoerPlaysCodes/kSTL`
- Run CMake with THESE flags: `cmake -S . -B build -DKSTL_FREESTANDING=ON`
- Build: `cmake --build build`

That's it! \
You *cannot* run tests in the freestanding build of kSTL.
