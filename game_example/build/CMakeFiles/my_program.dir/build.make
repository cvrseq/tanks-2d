# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aleksandrsuslin/desktop/game_example

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aleksandrsuslin/desktop/game_example/build

# Include any dependencies generated for this target.
include CMakeFiles/my_program.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/my_program.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/my_program.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_program.dir/flags.make

CMakeFiles/my_program.dir/codegen:
.PHONY : CMakeFiles/my_program.dir/codegen

CMakeFiles/my_program.dir/main.c.o: CMakeFiles/my_program.dir/flags.make
CMakeFiles/my_program.dir/main.c.o: /Users/aleksandrsuslin/desktop/game_example/main.c
CMakeFiles/my_program.dir/main.c.o: CMakeFiles/my_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/aleksandrsuslin/desktop/game_example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/my_program.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/my_program.dir/main.c.o -MF CMakeFiles/my_program.dir/main.c.o.d -o CMakeFiles/my_program.dir/main.c.o -c /Users/aleksandrsuslin/desktop/game_example/main.c

CMakeFiles/my_program.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/my_program.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/aleksandrsuslin/desktop/game_example/main.c > CMakeFiles/my_program.dir/main.c.i

CMakeFiles/my_program.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/my_program.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/aleksandrsuslin/desktop/game_example/main.c -o CMakeFiles/my_program.dir/main.c.s

# Object files for target my_program
my_program_OBJECTS = \
"CMakeFiles/my_program.dir/main.c.o"

# External object files for target my_program
my_program_EXTERNAL_OBJECTS =

my_program: CMakeFiles/my_program.dir/main.c.o
my_program: CMakeFiles/my_program.dir/build.make
my_program: /opt/homebrew/Cellar/raylib/5.5/lib/libraylib.dylib
my_program: CMakeFiles/my_program.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/aleksandrsuslin/desktop/game_example/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable my_program"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_program.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_program.dir/build: my_program
.PHONY : CMakeFiles/my_program.dir/build

CMakeFiles/my_program.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_program.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_program.dir/clean

CMakeFiles/my_program.dir/depend:
	cd /Users/aleksandrsuslin/desktop/game_example/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aleksandrsuslin/desktop/game_example /Users/aleksandrsuslin/desktop/game_example /Users/aleksandrsuslin/desktop/game_example/build /Users/aleksandrsuslin/desktop/game_example/build /Users/aleksandrsuslin/desktop/game_example/build/CMakeFiles/my_program.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/my_program.dir/depend

