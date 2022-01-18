cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Inculog)

add_compile_options (-fdiagnostics-color=always)
add_compile_options (-Wfatal-errors)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/W4")
    message("Compiling with: Microsoft Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-Wall" "-Wextra" "-pedantic")
    message("Compiling with: GCC Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #add_compile_options("-Weverything")
    add_compile_options(
            "-Wall"
            "-Wextra"
            "-pedantic"
            "-Wfatal-errors"
            "-Wno-c++98-compat"
            "-Wno-c++98-c++11-compat-binary-literal"
            "-Wno-c++98-compat-pedantic"
    )
    message("Compiling with: Clang Compiler")
endif()



find_package(fmt)
find_package(Threads)


add_executable(Inculog
    src/main.cpp src/CANManager.h)

add_subdirectory(toxic_spokes)

target_link_libraries(Inculog PRIVATE
    toxic_spokes::sockets
    fmt::fmt-header-only
    Threads::Threads
    )
