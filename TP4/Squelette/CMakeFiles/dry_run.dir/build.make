# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette

# Include any dependencies generated for this target.
include CMakeFiles/dry_run.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dry_run.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dry_run.dir/flags.make

CMakeFiles/dry_run.dir/src/bench.c.o: CMakeFiles/dry_run.dir/flags.make
CMakeFiles/dry_run.dir/src/bench.c.o: src/bench.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dry_run.dir/src/bench.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dry_run.dir/src/bench.c.o   -c /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/bench.c

CMakeFiles/dry_run.dir/src/bench.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dry_run.dir/src/bench.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/bench.c > CMakeFiles/dry_run.dir/src/bench.c.i

CMakeFiles/dry_run.dir/src/bench.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dry_run.dir/src/bench.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/bench.c -o CMakeFiles/dry_run.dir/src/bench.c.s

CMakeFiles/dry_run.dir/src/bench.c.o.requires:

.PHONY : CMakeFiles/dry_run.dir/src/bench.c.o.requires

CMakeFiles/dry_run.dir/src/bench.c.o.provides: CMakeFiles/dry_run.dir/src/bench.c.o.requires
	$(MAKE) -f CMakeFiles/dry_run.dir/build.make CMakeFiles/dry_run.dir/src/bench.c.o.provides.build
.PHONY : CMakeFiles/dry_run.dir/src/bench.c.o.provides

CMakeFiles/dry_run.dir/src/bench.c.o.provides.build: CMakeFiles/dry_run.dir/src/bench.c.o


CMakeFiles/dry_run.dir/src/matvec.c.o: CMakeFiles/dry_run.dir/flags.make
CMakeFiles/dry_run.dir/src/matvec.c.o: src/matvec.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/dry_run.dir/src/matvec.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dry_run.dir/src/matvec.c.o   -c /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/matvec.c

CMakeFiles/dry_run.dir/src/matvec.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dry_run.dir/src/matvec.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/matvec.c > CMakeFiles/dry_run.dir/src/matvec.c.i

CMakeFiles/dry_run.dir/src/matvec.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dry_run.dir/src/matvec.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/src/matvec.c -o CMakeFiles/dry_run.dir/src/matvec.c.s

CMakeFiles/dry_run.dir/src/matvec.c.o.requires:

.PHONY : CMakeFiles/dry_run.dir/src/matvec.c.o.requires

CMakeFiles/dry_run.dir/src/matvec.c.o.provides: CMakeFiles/dry_run.dir/src/matvec.c.o.requires
	$(MAKE) -f CMakeFiles/dry_run.dir/build.make CMakeFiles/dry_run.dir/src/matvec.c.o.provides.build
.PHONY : CMakeFiles/dry_run.dir/src/matvec.c.o.provides

CMakeFiles/dry_run.dir/src/matvec.c.o.provides.build: CMakeFiles/dry_run.dir/src/matvec.c.o


# Object files for target dry_run
dry_run_OBJECTS = \
"CMakeFiles/dry_run.dir/src/bench.c.o" \
"CMakeFiles/dry_run.dir/src/matvec.c.o"

# External object files for target dry_run
dry_run_EXTERNAL_OBJECTS =

bin/dry_run: CMakeFiles/dry_run.dir/src/bench.c.o
bin/dry_run: CMakeFiles/dry_run.dir/src/matvec.c.o
bin/dry_run: CMakeFiles/dry_run.dir/build.make
bin/dry_run: CMakeFiles/dry_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable bin/dry_run"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dry_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dry_run.dir/build: bin/dry_run

.PHONY : CMakeFiles/dry_run.dir/build

CMakeFiles/dry_run.dir/requires: CMakeFiles/dry_run.dir/src/bench.c.o.requires
CMakeFiles/dry_run.dir/requires: CMakeFiles/dry_run.dir/src/matvec.c.o.requires

.PHONY : CMakeFiles/dry_run.dir/requires

CMakeFiles/dry_run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dry_run.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dry_run.dir/clean

CMakeFiles/dry_run.dir/depend:
	cd /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette /home/eleves/promo20/info/lrolan/Bureau/arcpar/TP4/Squelette/CMakeFiles/dry_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dry_run.dir/depend

