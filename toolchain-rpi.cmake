if("${RASPBERRY_VERSION}" STREQUAL "")
    set(RASPBERRY_VERSION 4)
endif()

# Get sysroot with:
# rsync -vR --progress -rl --delete-after --safe-links <whatever_user>@<whatever_ip>:/{lib,usr,opt/vc/lib} <whatever_sysroot>/sysroot
# Usage:
# Note: full path to sysroot!!!!!!!!!!
# cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_TOOLCHAIN_FILE=<whatever>/toolchain-rpi.cmake -DCMAKE_SYSROOT=<whatever_sysroot>/sysroot

#prevent warning
if(NOT ${CMAKE_TOOLCHAIN_FILE})
endif()

message(STATUS "Using sysroot path: ${CMAKE_SYSROOT}")

set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_C_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_CXX_COMPILER_TARGET aarch64-linux-gnu)

set(toolchain_library_dirs
    ${CMAKE_SYSROOT}/usr/lib
)

list(TRANSFORM toolchain_library_dirs PREPEND -L OUTPUT_VARIABLE toolchain_linker_lib_flags)
list(TRANSFORM toolchain_library_dirs PREPEND -Wl,-rpath-link, OUTPUT_VARIABLE toolchain_linker_lib_flags_tmp)
list(APPEND toolchain_linker_lib_flags ${toolchain_linker_lib_flags_tmp})

if(RASPBERRY_VERSION EQUAL 4)
    set(toolchain_arch armv8)
    set(toolchain_cpu_flags -mcpu=cortex-a72)
elseif(RASPBERRY_VERSION EQUAL 3)
    set(toolchain_arch armv7)
    set(toolchain_cpu_flags -mcpu=cortex-a53)
else()
    message(FATAL_ERROR "pi version ${RASPBERRY_VERSION} not supported")
endif()

list(JOIN toolchain_arch " " CMAKE_SYSTEM_PROCESSOR)
list(JOIN toolchain_cpu_flags " " CMAKE_C_FLAGS)
list(JOIN toolchain_cpu_flags " " CMAKE_CXX_FLAGS)

if(${CMAKE_CXX_COMPILER} MATCHES clang)
    list(APPEND toolchain_linker_lib_flags -fuse-ld=lld)
endif()

list(JOIN toolchain_linker_lib_flags " " CMAKE_EXE_LINKER_FLAGS)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

