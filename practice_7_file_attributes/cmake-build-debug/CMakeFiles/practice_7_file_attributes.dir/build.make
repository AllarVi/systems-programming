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
CMAKE_SOURCE_DIR = "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/practice_7_file_attributes.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/practice_7_file_attributes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/practice_7_file_attributes.dir/flags.make

CMakeFiles/practice_7_file_attributes.dir/main.c.o: CMakeFiles/practice_7_file_attributes.dir/flags.make
CMakeFiles/practice_7_file_attributes.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/practice_7_file_attributes.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/practice_7_file_attributes.dir/main.c.o   -c "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/main.c"

CMakeFiles/practice_7_file_attributes.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/practice_7_file_attributes.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/main.c" > CMakeFiles/practice_7_file_attributes.dir/main.c.i

CMakeFiles/practice_7_file_attributes.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/practice_7_file_attributes.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/main.c" -o CMakeFiles/practice_7_file_attributes.dir/main.c.s

# Object files for target practice_7_file_attributes
practice_7_file_attributes_OBJECTS = \
"CMakeFiles/practice_7_file_attributes.dir/main.c.o"

# External object files for target practice_7_file_attributes
practice_7_file_attributes_EXTERNAL_OBJECTS =

practice_7_file_attributes: CMakeFiles/practice_7_file_attributes.dir/main.c.o
practice_7_file_attributes: CMakeFiles/practice_7_file_attributes.dir/build.make
practice_7_file_attributes: CMakeFiles/practice_7_file_attributes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable practice_7_file_attributes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/practice_7_file_attributes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/practice_7_file_attributes.dir/build: practice_7_file_attributes

.PHONY : CMakeFiles/practice_7_file_attributes.dir/build

CMakeFiles/practice_7_file_attributes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/practice_7_file_attributes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/practice_7_file_attributes.dir/clean

CMakeFiles/practice_7_file_attributes.dir/depend:
	cd "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug" "/Users/allarviinamae/EduWorkspace/(ITI8510)SystemsProgramming/practice_7_file_attributes/cmake-build-debug/CMakeFiles/practice_7_file_attributes.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/practice_7_file_attributes.dir/depend

