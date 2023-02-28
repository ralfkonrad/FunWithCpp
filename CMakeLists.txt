cmake_minimum_required(VERSION 3.22.0)
project(FunWithCpp VERSION 0.0.1)

option(RKE_USE_CLANG_TIDY "Enables clang tidy. OFF by default." OFF)
option(RKE_COMPILE_WARNING_AS_ERROR "If turned ON, treats all warnings as errors. OFF by default." OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 17 or higher")
endif ()

if (MSVC)
    # warning level 4
    add_compile_options(-W4)
else ()
    # lots of warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

if (RKE_COMPILE_WARNING_AS_ERROR)
    # all compiler warnings as errors
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        # since v3.24 cmake can set all compiler warnings as errors itself
        set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
    else ()
        # or set them manually
        if (MSVC)
            add_compile_options(-WX)
        else ()
            add_compile_options(-Werror)
        endif ()
    endif ()
endif ()

if (RKE_USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
            clang-tidy;
            -header-filter=.;
            -fix;)
endif ()

add_subdirectory(FunWithMoving)

enable_testing()
