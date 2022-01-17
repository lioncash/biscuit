cmake_minimum_required(VERSION 3.15)
project(biscuit VERSION 0.5.1)

include(CTest)

# Source directories
add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
