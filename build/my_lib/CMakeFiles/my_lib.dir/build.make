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
include my_lib/CMakeFiles/my_lib.dir/depend.make

# Include the progress variables for this target.
include my_lib/CMakeFiles/my_lib.dir/progress.make

# Include the compile flags for this target's objects.
include my_lib/CMakeFiles/my_lib.dir/flags.make

my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.o: my_lib/CMakeFiles/my_lib.dir/flags.make
my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.o: ../my_lib/src/vector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.o"
	cd /home/gexam/internship_experimentals/build/my_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/my_lib.dir/src/vector.cpp.o -c /home/gexam/internship_experimentals/my_lib/src/vector.cpp

my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/my_lib.dir/src/vector.cpp.i"
	cd /home/gexam/internship_experimentals/build/my_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gexam/internship_experimentals/my_lib/src/vector.cpp > CMakeFiles/my_lib.dir/src/vector.cpp.i

my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/my_lib.dir/src/vector.cpp.s"
	cd /home/gexam/internship_experimentals/build/my_lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gexam/internship_experimentals/my_lib/src/vector.cpp -o CMakeFiles/my_lib.dir/src/vector.cpp.s

# Object files for target my_lib
my_lib_OBJECTS = \
"CMakeFiles/my_lib.dir/src/vector.cpp.o"

# External object files for target my_lib
my_lib_EXTERNAL_OBJECTS =

my_lib/libmy_lib.a: my_lib/CMakeFiles/my_lib.dir/src/vector.cpp.o
my_lib/libmy_lib.a: my_lib/CMakeFiles/my_lib.dir/build.make
my_lib/libmy_lib.a: my_lib/CMakeFiles/my_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmy_lib.a"
	cd /home/gexam/internship_experimentals/build/my_lib && $(CMAKE_COMMAND) -P CMakeFiles/my_lib.dir/cmake_clean_target.cmake
	cd /home/gexam/internship_experimentals/build/my_lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
my_lib/CMakeFiles/my_lib.dir/build: my_lib/libmy_lib.a

.PHONY : my_lib/CMakeFiles/my_lib.dir/build

my_lib/CMakeFiles/my_lib.dir/clean:
	cd /home/gexam/internship_experimentals/build/my_lib && $(CMAKE_COMMAND) -P CMakeFiles/my_lib.dir/cmake_clean.cmake
.PHONY : my_lib/CMakeFiles/my_lib.dir/clean

my_lib/CMakeFiles/my_lib.dir/depend:
	cd /home/gexam/internship_experimentals/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gexam/internship_experimentals /home/gexam/internship_experimentals/my_lib /home/gexam/internship_experimentals/build /home/gexam/internship_experimentals/build/my_lib /home/gexam/internship_experimentals/build/my_lib/CMakeFiles/my_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : my_lib/CMakeFiles/my_lib.dir/depend
