# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/practice_5_threads.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/practice_5_threads.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/practice_5_threads.dir/flags.make

CMakeFiles/practice_5_threads.dir/threaded_hello.c.o: CMakeFiles/practice_5_threads.dir/flags.make
CMakeFiles/practice_5_threads.dir/threaded_hello.c.o: ../threaded_hello.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/practice_5_threads.dir/threaded_hello.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/practice_5_threads.dir/threaded_hello.c.o   -c "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/threaded_hello.c"

CMakeFiles/practice_5_threads.dir/threaded_hello.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/practice_5_threads.dir/threaded_hello.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/threaded_hello.c" > CMakeFiles/practice_5_threads.dir/threaded_hello.c.i

CMakeFiles/practice_5_threads.dir/threaded_hello.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/practice_5_threads.dir/threaded_hello.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/threaded_hello.c" -o CMakeFiles/practice_5_threads.dir/threaded_hello.c.s

CMakeFiles/practice_5_threads.dir/join.c.o: CMakeFiles/practice_5_threads.dir/flags.make
CMakeFiles/practice_5_threads.dir/join.c.o: ../join.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/practice_5_threads.dir/join.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/practice_5_threads.dir/join.c.o   -c "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/join.c"

CMakeFiles/practice_5_threads.dir/join.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/practice_5_threads.dir/join.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/join.c" > CMakeFiles/practice_5_threads.dir/join.c.i

CMakeFiles/practice_5_threads.dir/join.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/practice_5_threads.dir/join.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/join.c" -o CMakeFiles/practice_5_threads.dir/join.c.s

# Object files for target practice_5_threads
practice_5_threads_OBJECTS = \
"CMakeFiles/practice_5_threads.dir/threaded_hello.c.o" \
"CMakeFiles/practice_5_threads.dir/join.c.o"

# External object files for target practice_5_threads
practice_5_threads_EXTERNAL_OBJECTS =

practice_5_threads: CMakeFiles/practice_5_threads.dir/threaded_hello.c.o
practice_5_threads: CMakeFiles/practice_5_threads.dir/join.c.o
practice_5_threads: CMakeFiles/practice_5_threads.dir/build.make
practice_5_threads: CMakeFiles/practice_5_threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable practice_5_threads"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/practice_5_threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/practice_5_threads.dir/build: practice_5_threads

.PHONY : CMakeFiles/practice_5_threads.dir/build

CMakeFiles/practice_5_threads.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/practice_5_threads.dir/cmake_clean.cmake
.PHONY : CMakeFiles/practice_5_threads.dir/clean

CMakeFiles/practice_5_threads.dir/depend:
	cd "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_5_threads/cmake-build-debug/CMakeFiles/practice_5_threads.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/practice_5_threads.dir/depend

