# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = D:/dulred/dev/env/cmake/bin/cmake.exe

# The command to remove a file.
RM = D:/dulred/dev/env/cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/dulred/dev/source/c++/vscode/xxPlayer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/dulred/dev/source/c++/vscode/xxPlayer/build

# Include any dependencies generated for this target.
include xxPlayerTest/CMakeFiles/xxPlayerTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include xxPlayerTest/CMakeFiles/xxPlayerTest.dir/compiler_depend.make

# Include the progress variables for this target.
include xxPlayerTest/CMakeFiles/xxPlayerTest.dir/progress.make

# Include the compile flags for this target's objects.
include xxPlayerTest/CMakeFiles/xxPlayerTest.dir/flags.make

xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/flags.make
xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/includes_CXX.rsp
xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj: D:/dulred/dev/source/c++/vscode/xxPlayer/xxPlayerTest/main.cpp
xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:/dulred/dev/source/c++/vscode/xxPlayer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj"
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/software/Qt6.5.3/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj -MF CMakeFiles/xxPlayerTest.dir/main.cpp.obj.d -o CMakeFiles/xxPlayerTest.dir/main.cpp.obj -c D:/dulred/dev/source/c++/vscode/xxPlayer/xxPlayerTest/main.cpp

xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/xxPlayerTest.dir/main.cpp.i"
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/software/Qt6.5.3/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:/dulred/dev/source/c++/vscode/xxPlayer/xxPlayerTest/main.cpp > CMakeFiles/xxPlayerTest.dir/main.cpp.i

xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/xxPlayerTest.dir/main.cpp.s"
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/software/Qt6.5.3/Tools/mingw1120_64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:/dulred/dev/source/c++/vscode/xxPlayer/xxPlayerTest/main.cpp -o CMakeFiles/xxPlayerTest.dir/main.cpp.s

# Object files for target xxPlayerTest
xxPlayerTest_OBJECTS = \
"CMakeFiles/xxPlayerTest.dir/main.cpp.obj"

# External object files for target xxPlayerTest
xxPlayerTest_EXTERNAL_OBJECTS =

xxPlayerTest/xxPlayerTest.exe: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/main.cpp.obj
xxPlayerTest/xxPlayerTest.exe: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/build.make
xxPlayerTest/xxPlayerTest.exe: xxThread/libxxThread.a
xxPlayerTest/xxPlayerTest.exe: xxAV/libxxAV.a
xxPlayerTest/xxPlayerTest.exe: xxPlayer/libxxPlayer.a
xxPlayerTest/xxPlayerTest.exe: xxThread/libxxThread.a
xxPlayerTest/xxPlayerTest.exe: xxAV/libxxAV.a
xxPlayerTest/xxPlayerTest.exe: xxGL/libxxGL.a
xxPlayerTest/xxPlayerTest.exe: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/linkLibs.rsp
xxPlayerTest/xxPlayerTest.exe: xxPlayerTest/CMakeFiles/xxPlayerTest.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:/dulred/dev/source/c++/vscode/xxPlayer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable xxPlayerTest.exe"
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/env/cmake/bin/cmake.exe -E rm -f CMakeFiles/xxPlayerTest.dir/objects.a
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/software/Qt6.5.3/Tools/mingw1120_64/bin/ar.exe qc CMakeFiles/xxPlayerTest.dir/objects.a @CMakeFiles/xxPlayerTest.dir/objects1.rsp
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && D:/dulred/dev/software/Qt6.5.3/Tools/mingw1120_64/bin/c++.exe -Wl,--whole-archive CMakeFiles/xxPlayerTest.dir/objects.a -Wl,--no-whole-archive -o xxPlayerTest.exe -Wl,--out-implib,libxxPlayerTest.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/xxPlayerTest.dir/linkLibs.rsp

# Rule to build all files generated by this target.
xxPlayerTest/CMakeFiles/xxPlayerTest.dir/build: xxPlayerTest/xxPlayerTest.exe
.PHONY : xxPlayerTest/CMakeFiles/xxPlayerTest.dir/build

xxPlayerTest/CMakeFiles/xxPlayerTest.dir/clean:
	cd D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest && $(CMAKE_COMMAND) -P CMakeFiles/xxPlayerTest.dir/cmake_clean.cmake
.PHONY : xxPlayerTest/CMakeFiles/xxPlayerTest.dir/clean

xxPlayerTest/CMakeFiles/xxPlayerTest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/dulred/dev/source/c++/vscode/xxPlayer D:/dulred/dev/source/c++/vscode/xxPlayer/xxPlayerTest D:/dulred/dev/source/c++/vscode/xxPlayer/build D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest D:/dulred/dev/source/c++/vscode/xxPlayer/build/xxPlayerTest/CMakeFiles/xxPlayerTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : xxPlayerTest/CMakeFiles/xxPlayerTest.dir/depend

