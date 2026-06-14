cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(Assembler VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Dynamisch alle Source- und Header-Dateien im src-Ordner (und Unterordnern) finden
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp" "src/*.h")

# Add the executable
add_executable(Assembler ${SOURCES} ${HEADERS})

# src-Ordner als Include-Verzeichnis angeben, damit Header-Pfade kürzer sein können
target_include_directories(Assembler PRIVATE src)
