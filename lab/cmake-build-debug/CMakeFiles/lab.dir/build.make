# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Github Desktop\compile\lab"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Github Desktop\compile\lab\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/lab.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/lab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab.dir/flags.make

CMakeFiles/lab.dir/main.c.obj: CMakeFiles/lab.dir/flags.make
CMakeFiles/lab.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Github Desktop\compile\lab\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab.dir/main.c.obj"
	D:\MinGW64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\lab.dir\main.c.obj -c "D:\Github Desktop\compile\lab\main.c"

CMakeFiles/lab.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab.dir/main.c.i"
	D:\MinGW64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "D:\Github Desktop\compile\lab\main.c" > CMakeFiles\lab.dir\main.c.i

CMakeFiles/lab.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab.dir/main.c.s"
	D:\MinGW64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "D:\Github Desktop\compile\lab\main.c" -o CMakeFiles\lab.dir\main.c.s

# Object files for target lab
lab_OBJECTS = \
"CMakeFiles/lab.dir/main.c.obj"

# External object files for target lab
lab_EXTERNAL_OBJECTS =

lab.exe: CMakeFiles/lab.dir/main.c.obj
lab.exe: CMakeFiles/lab.dir/build.make
lab.exe: CMakeFiles/lab.dir/linklibs.rsp
lab.exe: CMakeFiles/lab.dir/objects1.rsp
lab.exe: CMakeFiles/lab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\Github Desktop\compile\lab\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable lab.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab.dir/build: lab.exe
.PHONY : CMakeFiles/lab.dir/build

CMakeFiles/lab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab.dir/clean

CMakeFiles/lab.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Github Desktop\compile\lab" "D:\Github Desktop\compile\lab" "D:\Github Desktop\compile\lab\cmake-build-debug" "D:\Github Desktop\compile\lab\cmake-build-debug" "D:\Github Desktop\compile\lab\cmake-build-debug\CMakeFiles\lab.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/lab.dir/depend
