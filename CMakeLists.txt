cmake_minimum_required(VERSION 3.22.0)
project(FunWithCpp VERSION 0.0.1)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RKE_USE_CLANG_TIDY "Enables clang tidy. OFF by default." OFF)
option(RKE_COMPILE_WARNING_AS_ERROR "If turned ON, treats all warnings as errors. OFF by default." OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()

if (MSVC)
    add_compile_options(-W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

if (RKE_COMPILE_WARNING_AS_ERROR)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        message(STATUS "Using CMAKE_COMPILE_WARNING_AS_ERROR.")
        set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
    else ()
        message(STATUS "Setting COMPILE_WARNING_AS_ERROR manually.")
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
