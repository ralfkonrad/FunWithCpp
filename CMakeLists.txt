cmake_minimum_required(VERSION 3.28.0)

# Default build type for single-config generators (set this before project() command)
# For multi-config generators, such as Visual Studio, use: cmake --build . --config=<CONFIG>
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE
            STRING "Build type for single-config generators" FORCE)
endif ()

project(FunWithCpp VERSION 0.0.1)

option(RKE_USE_CLANG_TIDY "Enables clang tidy. OFF by default." OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif ()

if (CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 20 or higher")
endif ()

if (MSVC)
    # warning level 4
    add_compile_options(-W4)
else ()
    # lots of warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

if (RKE_USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
            clang-tidy;
            -header-filter=.;
            -fix;)
endif ()

add_subdirectory(FunWithMoving)

enable_testing()
