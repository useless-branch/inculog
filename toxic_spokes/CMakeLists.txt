cmake_minimum_required(VERSION 3.1)

project(toxic_spokes)

add_library(toxic_spokes_sockets
    src/toxic_spokes/detail/IPAddress.cpp
    src/toxic_spokes/detail/FileDescriptor.cpp
    src/toxic_spokes/detail/Socket_Impl.cpp
)
target_include_directories(toxic_spokes_sockets PUBLIC src)
target_compile_features(toxic_spokes_sockets PUBLIC cxx_std_17)

add_library(toxic_spokes::sockets ALIAS toxic_spokes_sockets)

