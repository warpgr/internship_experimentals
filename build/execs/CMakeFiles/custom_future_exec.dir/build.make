# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/gexam/internship_experimentals

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gexam/internship_experimentals/build

# Include any dependencies generated for this target.
include execs/CMakeFiles/custom_future_exec.dir/depend.make

# Include the progress variables for this target.
include execs/CMakeFiles/custom_future_exec.dir/progress.make

# Include the compile flags for this target's objects.
include execs/CMakeFiles/custom_future_exec.dir/flags.make

execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o: execs/CMakeFiles/custom_future_exec.dir/flags.make
execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o: ../execs/future_custom_exec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o"
	cd /home/gexam/internship_experimentals/build/execs && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o -c /home/gexam/internship_experimentals/execs/future_custom_exec.cpp

execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.i"
	cd /home/gexam/internship_experimentals/build/execs && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gexam/internship_experimentals/execs/future_custom_exec.cpp > CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.i

execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.s"
	cd /home/gexam/internship_experimentals/build/execs && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gexam/internship_experimentals/execs/future_custom_exec.cpp -o CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.s

# Object files for target custom_future_exec
custom_future_exec_OBJECTS = \
"CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o"

# External object files for target custom_future_exec
custom_future_exec_EXTERNAL_OBJECTS =

execs/custom_future_exec: execs/CMakeFiles/custom_future_exec.dir/future_custom_exec.cpp.o
execs/custom_future_exec: execs/CMakeFiles/custom_future_exec.dir/build.make
execs/custom_future_exec: concurrent/libcontainers.a
execs/custom_future_exec: execs/CMakeFiles/custom_future_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable custom_future_exec"
	cd /home/gexam/internship_experimentals/build/execs && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/custom_future_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
execs/CMakeFiles/custom_future_exec.dir/build: execs/custom_future_exec

.PHONY : execs/CMakeFiles/custom_future_exec.dir/build

execs/CMakeFiles/custom_future_exec.dir/clean:
	cd /home/gexam/internship_experimentals/build/execs && $(CMAKE_COMMAND) -P CMakeFiles/custom_future_exec.dir/cmake_clean.cmake
.PHONY : execs/CMakeFiles/custom_future_exec.dir/clean

execs/CMakeFiles/custom_future_exec.dir/depend:
	cd /home/gexam/internship_experimentals/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gexam/internship_experimentals /home/gexam/internship_experimentals/execs /home/gexam/internship_experimentals/build /home/gexam/internship_experimentals/build/execs /home/gexam/internship_experimentals/build/execs/CMakeFiles/custom_future_exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : execs/CMakeFiles/custom_future_exec.dir/depend
