# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/patrick/git/incusens/software/RS232_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/patrick/git/incusens/software/RS232_test/build

# Include any dependencies generated for this target.
include toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/compiler_depend.make

# Include the progress variables for this target.
include toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/progress.make

# Include the compile flags for this target's objects.
include toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/flags.make

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/flags.make
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o: ../toxic_spokes/src/toxic_spokes/detail/IPAddress.cpp
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/patrick/git/incusens/software/RS232_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o -MF CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o.d -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o -c /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/IPAddress.cpp

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.i"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/IPAddress.cpp > CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.i

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.s"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/IPAddress.cpp -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.s

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/flags.make
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o: ../toxic_spokes/src/toxic_spokes/detail/FileDescriptor.cpp
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/patrick/git/incusens/software/RS232_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o -MF CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o.d -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o -c /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/FileDescriptor.cpp

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.i"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/FileDescriptor.cpp > CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.i

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.s"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/FileDescriptor.cpp -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.s

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/flags.make
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o: ../toxic_spokes/src/toxic_spokes/detail/Socket_Impl.cpp
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/patrick/git/incusens/software/RS232_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o -MF CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o.d -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o -c /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/Socket_Impl.cpp

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.i"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/Socket_Impl.cpp > CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.i

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.s"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/patrick/git/incusens/software/RS232_test/toxic_spokes/src/toxic_spokes/detail/Socket_Impl.cpp -o CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.s

# Object files for target toxic_spokes_sockets
toxic_spokes_sockets_OBJECTS = \
"CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o" \
"CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o" \
"CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o"

# External object files for target toxic_spokes_sockets
toxic_spokes_sockets_EXTERNAL_OBJECTS =

toxic_spokes/libtoxic_spokes_sockets.a: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/IPAddress.cpp.o
toxic_spokes/libtoxic_spokes_sockets.a: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/FileDescriptor.cpp.o
toxic_spokes/libtoxic_spokes_sockets.a: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/src/toxic_spokes/detail/Socket_Impl.cpp.o
toxic_spokes/libtoxic_spokes_sockets.a: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/build.make
toxic_spokes/libtoxic_spokes_sockets.a: toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/patrick/git/incusens/software/RS232_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libtoxic_spokes_sockets.a"
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && $(CMAKE_COMMAND) -P CMakeFiles/toxic_spokes_sockets.dir/cmake_clean_target.cmake
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/toxic_spokes_sockets.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/build: toxic_spokes/libtoxic_spokes_sockets.a
.PHONY : toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/build

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/clean:
	cd /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes && $(CMAKE_COMMAND) -P CMakeFiles/toxic_spokes_sockets.dir/cmake_clean.cmake
.PHONY : toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/clean

toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/depend:
	cd /home/patrick/git/incusens/software/RS232_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/patrick/git/incusens/software/RS232_test /home/patrick/git/incusens/software/RS232_test/toxic_spokes /home/patrick/git/incusens/software/RS232_test/build /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes /home/patrick/git/incusens/software/RS232_test/build/toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : toxic_spokes/CMakeFiles/toxic_spokes_sockets.dir/depend
