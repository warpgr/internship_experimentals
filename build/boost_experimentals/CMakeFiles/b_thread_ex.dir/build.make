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
include boost_experimentals/CMakeFiles/b_thread_ex.dir/depend.make

# Include the progress variables for this target.
include boost_experimentals/CMakeFiles/b_thread_ex.dir/progress.make

# Include the compile flags for this target's objects.
include boost_experimentals/CMakeFiles/b_thread_ex.dir/flags.make

boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.o: boost_experimentals/CMakeFiles/b_thread_ex.dir/flags.make
boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.o: ../boost_experimentals/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.o"
	cd /home/gexam/internship_experimentals/build/boost_experimentals && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/b_thread_ex.dir/thread.cpp.o -c /home/gexam/internship_experimentals/boost_experimentals/thread.cpp

boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/b_thread_ex.dir/thread.cpp.i"
	cd /home/gexam/internship_experimentals/build/boost_experimentals && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gexam/internship_experimentals/boost_experimentals/thread.cpp > CMakeFiles/b_thread_ex.dir/thread.cpp.i

boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/b_thread_ex.dir/thread.cpp.s"
	cd /home/gexam/internship_experimentals/build/boost_experimentals && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gexam/internship_experimentals/boost_experimentals/thread.cpp -o CMakeFiles/b_thread_ex.dir/thread.cpp.s

# Object files for target b_thread_ex
b_thread_ex_OBJECTS = \
"CMakeFiles/b_thread_ex.dir/thread.cpp.o"

# External object files for target b_thread_ex
b_thread_ex_EXTERNAL_OBJECTS =

boost_experimentals/b_thread_ex: boost_experimentals/CMakeFiles/b_thread_ex.dir/thread.cpp.o
boost_experimentals/b_thread_ex: boost_experimentals/CMakeFiles/b_thread_ex.dir/build.make
boost_experimentals/b_thread_ex: /usr/local/lib/libboost_filesystem-mt-d-x64.so.1.80.0
boost_experimentals/b_thread_ex: /usr/local/lib/libboost_thread-mt-d-x64.so.1.80.0
boost_experimentals/b_thread_ex: /usr/local/lib/libboost_chrono-mt-d-x64.so.1.80.0
boost_experimentals/b_thread_ex: /usr/local/lib/libboost_atomic-mt-d-x64.so.1.80.0
boost_experimentals/b_thread_ex: boost_experimentals/CMakeFiles/b_thread_ex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gexam/internship_experimentals/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable b_thread_ex"
	cd /home/gexam/internship_experimentals/build/boost_experimentals && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/b_thread_ex.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
boost_experimentals/CMakeFiles/b_thread_ex.dir/build: boost_experimentals/b_thread_ex

.PHONY : boost_experimentals/CMakeFiles/b_thread_ex.dir/build

boost_experimentals/CMakeFiles/b_thread_ex.dir/clean:
	cd /home/gexam/internship_experimentals/build/boost_experimentals && $(CMAKE_COMMAND) -P CMakeFiles/b_thread_ex.dir/cmake_clean.cmake
.PHONY : boost_experimentals/CMakeFiles/b_thread_ex.dir/clean

boost_experimentals/CMakeFiles/b_thread_ex.dir/depend:
	cd /home/gexam/internship_experimentals/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gexam/internship_experimentals /home/gexam/internship_experimentals/boost_experimentals /home/gexam/internship_experimentals/build /home/gexam/internship_experimentals/build/boost_experimentals /home/gexam/internship_experimentals/build/boost_experimentals/CMakeFiles/b_thread_ex.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : boost_experimentals/CMakeFiles/b_thread_ex.dir/depend

