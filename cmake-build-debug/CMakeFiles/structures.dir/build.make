# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/oleg/IDES/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/oleg/IDES/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oleg/CLionProjects/structures

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oleg/CLionProjects/structures/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/structures.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/structures.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/structures.dir/flags.make

CMakeFiles/structures.dir/main.c.o: CMakeFiles/structures.dir/flags.make
CMakeFiles/structures.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oleg/CLionProjects/structures/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/structures.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/structures.dir/main.c.o   -c /home/oleg/CLionProjects/structures/main.c

CMakeFiles/structures.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/structures.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/oleg/CLionProjects/structures/main.c > CMakeFiles/structures.dir/main.c.i

CMakeFiles/structures.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/structures.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/oleg/CLionProjects/structures/main.c -o CMakeFiles/structures.dir/main.c.s

CMakeFiles/structures.dir/main.c.o.requires:

.PHONY : CMakeFiles/structures.dir/main.c.o.requires

CMakeFiles/structures.dir/main.c.o.provides: CMakeFiles/structures.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/structures.dir/build.make CMakeFiles/structures.dir/main.c.o.provides.build
.PHONY : CMakeFiles/structures.dir/main.c.o.provides

CMakeFiles/structures.dir/main.c.o.provides.build: CMakeFiles/structures.dir/main.c.o


# Object files for target structures
structures_OBJECTS = \
"CMakeFiles/structures.dir/main.c.o"

# External object files for target structures
structures_EXTERNAL_OBJECTS =

../structures: CMakeFiles/structures.dir/main.c.o
../structures: CMakeFiles/structures.dir/build.make
../structures: CMakeFiles/structures.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oleg/CLionProjects/structures/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../structures"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/structures.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/structures.dir/build: ../structures

.PHONY : CMakeFiles/structures.dir/build

CMakeFiles/structures.dir/requires: CMakeFiles/structures.dir/main.c.o.requires

.PHONY : CMakeFiles/structures.dir/requires

CMakeFiles/structures.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/structures.dir/cmake_clean.cmake
.PHONY : CMakeFiles/structures.dir/clean

CMakeFiles/structures.dir/depend:
	cd /home/oleg/CLionProjects/structures/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oleg/CLionProjects/structures /home/oleg/CLionProjects/structures /home/oleg/CLionProjects/structures/cmake-build-debug /home/oleg/CLionProjects/structures/cmake-build-debug /home/oleg/CLionProjects/structures/cmake-build-debug/CMakeFiles/structures.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/structures.dir/depend

