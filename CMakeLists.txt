cmake_minimum_required(VERSION 3.30)
set(PROJECT_NAME kstl)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(BINARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIRECTORY})
project(${PROJECT_NAME} CXX)
# Set CMake Build Type
set(CMAKE_BUILD_TYPE Debug)
option(BUILD_SHARED_LIBS "Build as a Shared Library" ON)
option(KSTL_FREESTANDING "Build as a Freestanding Static Library" OFF)
option(KSTL_TESTS "Build Tests" ON)
option(KSTL_FREESTANDING_X86 "Build as 32-bit x86" OFF)

option(KSTL_FLOATING_POINT_SUPPORTED "Support floating-point ")

set(KSTL_COMPILER_FLAGS -Wall -Wextra -Wpedantic -Werror=return-type -Werror=uninitialized -Werror=nonnull -Werror=null-dereference -Werror=strict-aliasing -Werror=undef -Werror=sign-compare -fno-omit-frame-pointer)

if (KSTL_FREESTANDING)
    if (KSTL_FREESTANDING_X86)
        message(STATUS "Builting as 32-bit x86")
        set(KSTL_COMPILER_FLAGS -m32 ${KSTL_COMPILER_FLAGS})
        add_link_options(
            -m32
        )
    endif()
    set(BUILD_SHARED_LIBS OFF)
    set(KSTL_TESTS OFF)
    message(STATUS "Building in Freestanding Mode")
    message(WARNING "Tests will be disabled")

    set(KSTL_COMPILER_FLAGS 
        ${KSTL_COMPILER_FLAGS} 
        -ffreestanding
        -fno-rtti
        -fno-exceptions
        -nostdlib
        -nodefaultlibs
        -mno-red-zone
        -fno-stack-protector
        -fno-pic
        -fno-pie
    )

    add_link_options(
        -no-pie
    )
endif()

add_library(${PROJECT_NAME}
    src/runtime.cpp
    src/string.cpp
    src/stdlib.cpp
)

if (KSTL_FREESTANDING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __KSTL_FREESTANDING__=1)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE ${KSTL_COMPILER_FLAGS})

target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_include_directories(${PROJECT_NAME} PRIVATE include)
# see, no dependencies!!!
target_link_libraries(${PROJECT_NAME} PRIVATE)

if (KSTL_TESTS)
    add_executable(tester
        tests/tester.cpp
    )

    target_include_directories(tester PRIVATE include)
    target_link_libraries(tester PRIVATE kstl)

    set(TEST_NAMES
        string/basic
        string/cstring
        string/edge_case_indexing
        string/iterators

        vector/basic
        vector/iterators
        vector/edge_case_indexing
        vector/resize_and_reserve
        vector/move_only_type

        unordered_map/basic
        unordered_map/iterators

        unique_ptr/basic

        optional/basic
    )

    add_library(log_vector STATIC
        tests/vector/log_vector.cpp
    )

    target_include_directories(log_vector PRIVATE
        include
        src/include
        tests
    )
    target_link_libraries(log_vector PRIVATE
        kstl
    )

    foreach(test_name IN LISTS TEST_NAMES)
        string(REPLACE "/" "_" test_target "${test_name}")

        get_filename_component(test_category "${test_name}" DIRECTORY)
        get_filename_component(test_binary "${test_name}" NAME)

        add_executable(${test_target}
            tests/common.cpp
            tests/${test_name}.cpp
        )

        target_include_directories(${test_target} PRIVATE
            include
            src/include
            tests
        )

        target_link_libraries(${test_target} PRIVATE
            kstl
            log_vector
        )

        set_target_properties(${test_target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_SOURCE_DIR}/bin/tests/${test_category}"

            OUTPUT_NAME
            "${test_binary}"
        )
    endforeach()
endif()
