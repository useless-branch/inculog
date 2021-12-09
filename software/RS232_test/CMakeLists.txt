cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RS232Tester)

add_compile_options (-fdiagnostics-color=always)
add_compile_options (-Wfatal-errors)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/W4")
    message("Compiling with: Microsoft Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-Wall" "-Wextra" "-pedantic")
    message("Compiling with: GCC Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options("-Weverything")
    add_compile_options("-Wall" "-Wextra" "-pedantic" "-Wfatal-errors" "-fsanitize=thread")
    message("Compiling with: Clang Compiler")
endif()



find_package(fmt)
find_package(Threads)


add_executable(RS232Tester 
    src/main.cpp
    )

add_subdirectory(toxic_spokes)

target_link_libraries(RS232Tester PRIVATE
    toxic_spokes::sockets
    fmt::fmt
    Threads::Threads
    )
