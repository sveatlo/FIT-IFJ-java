# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /opt/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proj1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/proj1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proj1.dir/flags.make

CMakeFiles/proj1.dir/src/callframe.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/callframe.c.o: ../src/callframe.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/proj1.dir/src/callframe.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/callframe.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callframe.c

CMakeFiles/proj1.dir/src/callframe.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/callframe.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callframe.c > CMakeFiles/proj1.dir/src/callframe.c.i

CMakeFiles/proj1.dir/src/callframe.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/callframe.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callframe.c -o CMakeFiles/proj1.dir/src/callframe.c.s

CMakeFiles/proj1.dir/src/callframe.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/callframe.c.o.requires

CMakeFiles/proj1.dir/src/callframe.c.o.provides: CMakeFiles/proj1.dir/src/callframe.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/callframe.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/callframe.c.o.provides

CMakeFiles/proj1.dir/src/callframe.c.o.provides.build: CMakeFiles/proj1.dir/src/callframe.c.o


CMakeFiles/proj1.dir/src/callstack.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/callstack.c.o: ../src/callstack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/proj1.dir/src/callstack.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/callstack.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callstack.c

CMakeFiles/proj1.dir/src/callstack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/callstack.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callstack.c > CMakeFiles/proj1.dir/src/callstack.c.i

CMakeFiles/proj1.dir/src/callstack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/callstack.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/callstack.c -o CMakeFiles/proj1.dir/src/callstack.c.s

CMakeFiles/proj1.dir/src/callstack.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/callstack.c.o.requires

CMakeFiles/proj1.dir/src/callstack.c.o.provides: CMakeFiles/proj1.dir/src/callstack.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/callstack.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/callstack.c.o.provides

CMakeFiles/proj1.dir/src/callstack.c.o.provides.build: CMakeFiles/proj1.dir/src/callstack.c.o


CMakeFiles/proj1.dir/src/context.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/context.c.o: ../src/context.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/proj1.dir/src/context.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/context.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/context.c

CMakeFiles/proj1.dir/src/context.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/context.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/context.c > CMakeFiles/proj1.dir/src/context.c.i

CMakeFiles/proj1.dir/src/context.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/context.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/context.c -o CMakeFiles/proj1.dir/src/context.c.s

CMakeFiles/proj1.dir/src/context.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/context.c.o.requires

CMakeFiles/proj1.dir/src/context.c.o.provides: CMakeFiles/proj1.dir/src/context.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/context.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/context.c.o.provides

CMakeFiles/proj1.dir/src/context.c.o.provides.build: CMakeFiles/proj1.dir/src/context.c.o


CMakeFiles/proj1.dir/src/error.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/error.c.o: ../src/error.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/proj1.dir/src/error.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/error.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/error.c

CMakeFiles/proj1.dir/src/error.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/error.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/error.c > CMakeFiles/proj1.dir/src/error.c.i

CMakeFiles/proj1.dir/src/error.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/error.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/error.c -o CMakeFiles/proj1.dir/src/error.c.s

CMakeFiles/proj1.dir/src/error.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/error.c.o.requires

CMakeFiles/proj1.dir/src/error.c.o.provides: CMakeFiles/proj1.dir/src/error.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/error.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/error.c.o.provides

CMakeFiles/proj1.dir/src/error.c.o.provides.build: CMakeFiles/proj1.dir/src/error.c.o


CMakeFiles/proj1.dir/src/expression.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/expression.c.o: ../src/expression.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/proj1.dir/src/expression.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/expression.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/expression.c

CMakeFiles/proj1.dir/src/expression.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/expression.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/expression.c > CMakeFiles/proj1.dir/src/expression.c.i

CMakeFiles/proj1.dir/src/expression.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/expression.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/expression.c -o CMakeFiles/proj1.dir/src/expression.c.s

CMakeFiles/proj1.dir/src/expression.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/expression.c.o.requires

CMakeFiles/proj1.dir/src/expression.c.o.provides: CMakeFiles/proj1.dir/src/expression.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/expression.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/expression.c.o.provides

CMakeFiles/proj1.dir/src/expression.c.o.provides.build: CMakeFiles/proj1.dir/src/expression.c.o


CMakeFiles/proj1.dir/src/ial.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/ial.c.o: ../src/ial.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/proj1.dir/src/ial.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/ial.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/ial.c

CMakeFiles/proj1.dir/src/ial.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/ial.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/ial.c > CMakeFiles/proj1.dir/src/ial.c.i

CMakeFiles/proj1.dir/src/ial.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/ial.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/ial.c -o CMakeFiles/proj1.dir/src/ial.c.s

CMakeFiles/proj1.dir/src/ial.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/ial.c.o.requires

CMakeFiles/proj1.dir/src/ial.c.o.provides: CMakeFiles/proj1.dir/src/ial.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/ial.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/ial.c.o.provides

CMakeFiles/proj1.dir/src/ial.c.o.provides.build: CMakeFiles/proj1.dir/src/ial.c.o


CMakeFiles/proj1.dir/src/inbuilt.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/inbuilt.c.o: ../src/inbuilt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/proj1.dir/src/inbuilt.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/inbuilt.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/inbuilt.c

CMakeFiles/proj1.dir/src/inbuilt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/inbuilt.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/inbuilt.c > CMakeFiles/proj1.dir/src/inbuilt.c.i

CMakeFiles/proj1.dir/src/inbuilt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/inbuilt.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/inbuilt.c -o CMakeFiles/proj1.dir/src/inbuilt.c.s

CMakeFiles/proj1.dir/src/inbuilt.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/inbuilt.c.o.requires

CMakeFiles/proj1.dir/src/inbuilt.c.o.provides: CMakeFiles/proj1.dir/src/inbuilt.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/inbuilt.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/inbuilt.c.o.provides

CMakeFiles/proj1.dir/src/inbuilt.c.o.provides.build: CMakeFiles/proj1.dir/src/inbuilt.c.o


CMakeFiles/proj1.dir/src/instruction.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/instruction.c.o: ../src/instruction.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/proj1.dir/src/instruction.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/instruction.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/instruction.c

CMakeFiles/proj1.dir/src/instruction.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/instruction.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/instruction.c > CMakeFiles/proj1.dir/src/instruction.c.i

CMakeFiles/proj1.dir/src/instruction.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/instruction.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/instruction.c -o CMakeFiles/proj1.dir/src/instruction.c.s

CMakeFiles/proj1.dir/src/instruction.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/instruction.c.o.requires

CMakeFiles/proj1.dir/src/instruction.c.o.provides: CMakeFiles/proj1.dir/src/instruction.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/instruction.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/instruction.c.o.provides

CMakeFiles/proj1.dir/src/instruction.c.o.provides.build: CMakeFiles/proj1.dir/src/instruction.c.o


CMakeFiles/proj1.dir/src/interpret.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/interpret.c.o: ../src/interpret.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/proj1.dir/src/interpret.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/interpret.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/interpret.c

CMakeFiles/proj1.dir/src/interpret.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/interpret.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/interpret.c > CMakeFiles/proj1.dir/src/interpret.c.i

CMakeFiles/proj1.dir/src/interpret.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/interpret.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/interpret.c -o CMakeFiles/proj1.dir/src/interpret.c.s

CMakeFiles/proj1.dir/src/interpret.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/interpret.c.o.requires

CMakeFiles/proj1.dir/src/interpret.c.o.provides: CMakeFiles/proj1.dir/src/interpret.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/interpret.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/interpret.c.o.provides

CMakeFiles/proj1.dir/src/interpret.c.o.provides.build: CMakeFiles/proj1.dir/src/interpret.c.o


CMakeFiles/proj1.dir/src/list.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/list.c.o: ../src/list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/proj1.dir/src/list.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/list.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/list.c

CMakeFiles/proj1.dir/src/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/list.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/list.c > CMakeFiles/proj1.dir/src/list.c.i

CMakeFiles/proj1.dir/src/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/list.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/list.c -o CMakeFiles/proj1.dir/src/list.c.s

CMakeFiles/proj1.dir/src/list.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/list.c.o.requires

CMakeFiles/proj1.dir/src/list.c.o.provides: CMakeFiles/proj1.dir/src/list.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/list.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/list.c.o.provides

CMakeFiles/proj1.dir/src/list.c.o.provides.build: CMakeFiles/proj1.dir/src/list.c.o


CMakeFiles/proj1.dir/src/main.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/proj1.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/main.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/main.c

CMakeFiles/proj1.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/main.c > CMakeFiles/proj1.dir/src/main.c.i

CMakeFiles/proj1.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/main.c -o CMakeFiles/proj1.dir/src/main.c.s

CMakeFiles/proj1.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/main.c.o.requires

CMakeFiles/proj1.dir/src/main.c.o.provides: CMakeFiles/proj1.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/main.c.o.provides

CMakeFiles/proj1.dir/src/main.c.o.provides.build: CMakeFiles/proj1.dir/src/main.c.o


CMakeFiles/proj1.dir/src/parser.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/parser.c.o: ../src/parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/proj1.dir/src/parser.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/parser.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/parser.c

CMakeFiles/proj1.dir/src/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/parser.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/parser.c > CMakeFiles/proj1.dir/src/parser.c.i

CMakeFiles/proj1.dir/src/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/parser.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/parser.c -o CMakeFiles/proj1.dir/src/parser.c.s

CMakeFiles/proj1.dir/src/parser.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/parser.c.o.requires

CMakeFiles/proj1.dir/src/parser.c.o.provides: CMakeFiles/proj1.dir/src/parser.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/parser.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/parser.c.o.provides

CMakeFiles/proj1.dir/src/parser.c.o.provides.build: CMakeFiles/proj1.dir/src/parser.c.o


CMakeFiles/proj1.dir/src/precedence_table.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/precedence_table.c.o: ../src/precedence_table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/proj1.dir/src/precedence_table.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/precedence_table.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/precedence_table.c

CMakeFiles/proj1.dir/src/precedence_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/precedence_table.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/precedence_table.c > CMakeFiles/proj1.dir/src/precedence_table.c.i

CMakeFiles/proj1.dir/src/precedence_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/precedence_table.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/precedence_table.c -o CMakeFiles/proj1.dir/src/precedence_table.c.s

CMakeFiles/proj1.dir/src/precedence_table.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/precedence_table.c.o.requires

CMakeFiles/proj1.dir/src/precedence_table.c.o.provides: CMakeFiles/proj1.dir/src/precedence_table.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/precedence_table.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/precedence_table.c.o.provides

CMakeFiles/proj1.dir/src/precedence_table.c.o.provides.build: CMakeFiles/proj1.dir/src/precedence_table.c.o


CMakeFiles/proj1.dir/src/scanner.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/scanner.c.o: ../src/scanner.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/proj1.dir/src/scanner.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/scanner.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner.c

CMakeFiles/proj1.dir/src/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/scanner.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner.c > CMakeFiles/proj1.dir/src/scanner.c.i

CMakeFiles/proj1.dir/src/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/scanner.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner.c -o CMakeFiles/proj1.dir/src/scanner.c.s

CMakeFiles/proj1.dir/src/scanner.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/scanner.c.o.requires

CMakeFiles/proj1.dir/src/scanner.c.o.provides: CMakeFiles/proj1.dir/src/scanner.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/scanner.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/scanner.c.o.provides

CMakeFiles/proj1.dir/src/scanner.c.o.provides.build: CMakeFiles/proj1.dir/src/scanner.c.o


CMakeFiles/proj1.dir/src/scanner_token.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/scanner_token.c.o: ../src/scanner_token.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/proj1.dir/src/scanner_token.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/scanner_token.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner_token.c

CMakeFiles/proj1.dir/src/scanner_token.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/scanner_token.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner_token.c > CMakeFiles/proj1.dir/src/scanner_token.c.i

CMakeFiles/proj1.dir/src/scanner_token.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/scanner_token.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/scanner_token.c -o CMakeFiles/proj1.dir/src/scanner_token.c.s

CMakeFiles/proj1.dir/src/scanner_token.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/scanner_token.c.o.requires

CMakeFiles/proj1.dir/src/scanner_token.c.o.provides: CMakeFiles/proj1.dir/src/scanner_token.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/scanner_token.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/scanner_token.c.o.provides

CMakeFiles/proj1.dir/src/scanner_token.c.o.provides.build: CMakeFiles/proj1.dir/src/scanner_token.c.o


CMakeFiles/proj1.dir/src/stack.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/stack.c.o: ../src/stack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/proj1.dir/src/stack.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/stack.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/stack.c

CMakeFiles/proj1.dir/src/stack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/stack.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/stack.c > CMakeFiles/proj1.dir/src/stack.c.i

CMakeFiles/proj1.dir/src/stack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/stack.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/stack.c -o CMakeFiles/proj1.dir/src/stack.c.s

CMakeFiles/proj1.dir/src/stack.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/stack.c.o.requires

CMakeFiles/proj1.dir/src/stack.c.o.provides: CMakeFiles/proj1.dir/src/stack.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/stack.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/stack.c.o.provides

CMakeFiles/proj1.dir/src/stack.c.o.provides.build: CMakeFiles/proj1.dir/src/stack.c.o


CMakeFiles/proj1.dir/src/string.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/string.c.o: ../src/string.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/proj1.dir/src/string.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/string.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/string.c

CMakeFiles/proj1.dir/src/string.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/string.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/string.c > CMakeFiles/proj1.dir/src/string.c.i

CMakeFiles/proj1.dir/src/string.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/string.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/string.c -o CMakeFiles/proj1.dir/src/string.c.s

CMakeFiles/proj1.dir/src/string.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/string.c.o.requires

CMakeFiles/proj1.dir/src/string.c.o.provides: CMakeFiles/proj1.dir/src/string.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/string.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/string.c.o.provides

CMakeFiles/proj1.dir/src/string.c.o.provides.build: CMakeFiles/proj1.dir/src/string.c.o


CMakeFiles/proj1.dir/src/symbol.c.o: CMakeFiles/proj1.dir/flags.make
CMakeFiles/proj1.dir/src/symbol.c.o: ../src/symbol.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building C object CMakeFiles/proj1.dir/src/symbol.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proj1.dir/src/symbol.c.o   -c /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/symbol.c

CMakeFiles/proj1.dir/src/symbol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proj1.dir/src/symbol.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/symbol.c > CMakeFiles/proj1.dir/src/symbol.c.i

CMakeFiles/proj1.dir/src/symbol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proj1.dir/src/symbol.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/src/symbol.c -o CMakeFiles/proj1.dir/src/symbol.c.s

CMakeFiles/proj1.dir/src/symbol.c.o.requires:

.PHONY : CMakeFiles/proj1.dir/src/symbol.c.o.requires

CMakeFiles/proj1.dir/src/symbol.c.o.provides: CMakeFiles/proj1.dir/src/symbol.c.o.requires
	$(MAKE) -f CMakeFiles/proj1.dir/build.make CMakeFiles/proj1.dir/src/symbol.c.o.provides.build
.PHONY : CMakeFiles/proj1.dir/src/symbol.c.o.provides

CMakeFiles/proj1.dir/src/symbol.c.o.provides.build: CMakeFiles/proj1.dir/src/symbol.c.o


# Object files for target proj1
proj1_OBJECTS = \
"CMakeFiles/proj1.dir/src/callframe.c.o" \
"CMakeFiles/proj1.dir/src/callstack.c.o" \
"CMakeFiles/proj1.dir/src/context.c.o" \
"CMakeFiles/proj1.dir/src/error.c.o" \
"CMakeFiles/proj1.dir/src/expression.c.o" \
"CMakeFiles/proj1.dir/src/ial.c.o" \
"CMakeFiles/proj1.dir/src/inbuilt.c.o" \
"CMakeFiles/proj1.dir/src/instruction.c.o" \
"CMakeFiles/proj1.dir/src/interpret.c.o" \
"CMakeFiles/proj1.dir/src/list.c.o" \
"CMakeFiles/proj1.dir/src/main.c.o" \
"CMakeFiles/proj1.dir/src/parser.c.o" \
"CMakeFiles/proj1.dir/src/precedence_table.c.o" \
"CMakeFiles/proj1.dir/src/scanner.c.o" \
"CMakeFiles/proj1.dir/src/scanner_token.c.o" \
"CMakeFiles/proj1.dir/src/stack.c.o" \
"CMakeFiles/proj1.dir/src/string.c.o" \
"CMakeFiles/proj1.dir/src/symbol.c.o"

# External object files for target proj1
proj1_EXTERNAL_OBJECTS =

proj1: CMakeFiles/proj1.dir/src/callframe.c.o
proj1: CMakeFiles/proj1.dir/src/callstack.c.o
proj1: CMakeFiles/proj1.dir/src/context.c.o
proj1: CMakeFiles/proj1.dir/src/error.c.o
proj1: CMakeFiles/proj1.dir/src/expression.c.o
proj1: CMakeFiles/proj1.dir/src/ial.c.o
proj1: CMakeFiles/proj1.dir/src/inbuilt.c.o
proj1: CMakeFiles/proj1.dir/src/instruction.c.o
proj1: CMakeFiles/proj1.dir/src/interpret.c.o
proj1: CMakeFiles/proj1.dir/src/list.c.o
proj1: CMakeFiles/proj1.dir/src/main.c.o
proj1: CMakeFiles/proj1.dir/src/parser.c.o
proj1: CMakeFiles/proj1.dir/src/precedence_table.c.o
proj1: CMakeFiles/proj1.dir/src/scanner.c.o
proj1: CMakeFiles/proj1.dir/src/scanner_token.c.o
proj1: CMakeFiles/proj1.dir/src/stack.c.o
proj1: CMakeFiles/proj1.dir/src/string.c.o
proj1: CMakeFiles/proj1.dir/src/symbol.c.o
proj1: CMakeFiles/proj1.dir/build.make
proj1: CMakeFiles/proj1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking C executable proj1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proj1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proj1.dir/build: proj1

.PHONY : CMakeFiles/proj1.dir/build

CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/callframe.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/callstack.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/context.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/error.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/expression.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/ial.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/inbuilt.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/instruction.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/interpret.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/list.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/main.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/parser.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/precedence_table.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/scanner.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/scanner_token.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/stack.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/string.c.o.requires
CMakeFiles/proj1.dir/requires: CMakeFiles/proj1.dir/src/symbol.c.o.requires

.PHONY : CMakeFiles/proj1.dir/requires

CMakeFiles/proj1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proj1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proj1.dir/clean

CMakeFiles/proj1.dir/depend:
	cd /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1 /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1 /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug /home/sveatlo/Documents/school/uni/assignments/2/winter/IFJ/proj1/cmake-build-debug/CMakeFiles/proj1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/proj1.dir/depend

