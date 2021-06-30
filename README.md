# Biscuit: RISC-V Runtime Code Generation Library

*RISC it for the biscuit*

An experimental runtime code generator for RISC-V. Emphasis on the experimental part.


## Implemented Extension Instructions

Includes both 32-bit and 64-bit instructions in the following:

- RVA
- RVC
- RVD
- RVF
- RVI
- RVM
- RVQ
- Zicsr
- Zifencei

Note that only extensions considered ratified will be implemented
as non-ratified documents are considerably more likely to have
large changes made to them, which makes maintaining instruction
APIs a little annoying.


## Dependencies

Biscuit requires no external dependencies for its library other than the C++ standard library. 
The tests, however, use the Catch2 testing library. This is included in tree so there's no need
to worry about installing it yourself if you wish to run said tests.


## Building Biscuit

1. Generate the build files for the project with CMake
2. Hit the build button in your IDE of choice, or run the relevant console command to build for the CMake generator you've chosen.
3. Done.


## Running Tests

1. Generate the build files for the project with CMake
2. Build the tests
3. Run the test executable directly, or enter `ctest` into your terminal.


## License

The library is licensed under the MIT license.

While it's not a requirement whatsoever, it'd be pretty neat if you told me that you found the library useful :-)