# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/dt/ParProg/lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dt/ParProg/lab3/build

# Include any dependencies generated for this target.
include CMakeFiles/lab3.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lab3.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lab3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab3.dir/flags.make

CMakeFiles/lab3.dir/lab3.c.o: CMakeFiles/lab3.dir/flags.make
CMakeFiles/lab3.dir/lab3.c.o: /home/dt/ParProg/lab3/lab3.c
CMakeFiles/lab3.dir/lab3.c.o: CMakeFiles/lab3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/dt/ParProg/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab3.dir/lab3.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lab3.dir/lab3.c.o -MF CMakeFiles/lab3.dir/lab3.c.o.d -o CMakeFiles/lab3.dir/lab3.c.o -c /home/dt/ParProg/lab3/lab3.c

CMakeFiles/lab3.dir/lab3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/lab3.dir/lab3.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dt/ParProg/lab3/lab3.c > CMakeFiles/lab3.dir/lab3.c.i

CMakeFiles/lab3.dir/lab3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/lab3.dir/lab3.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dt/ParProg/lab3/lab3.c -o CMakeFiles/lab3.dir/lab3.c.s

# Object files for target lab3
lab3_OBJECTS = \
"CMakeFiles/lab3.dir/lab3.c.o"

# External object files for target lab3
lab3_EXTERNAL_OBJECTS =

lab3: CMakeFiles/lab3.dir/lab3.c.o
lab3: CMakeFiles/lab3.dir/build.make
lab3: /usr/lib/gcc/x86_64-linux-gnu/13/libgomp.so
lab3: /usr/lib/x86_64-linux-gnu/libpthread.a
lab3: CMakeFiles/lab3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/dt/ParProg/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable lab3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab3.dir/build: lab3
.PHONY : CMakeFiles/lab3.dir/build

CMakeFiles/lab3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab3.dir/clean

CMakeFiles/lab3.dir/depend:
	cd /home/dt/ParProg/lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dt/ParProg/lab3 /home/dt/ParProg/lab3 /home/dt/ParProg/lab3/build /home/dt/ParProg/lab3/build /home/dt/ParProg/lab3/build/CMakeFiles/lab3.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/lab3.dir/depend

