cmake_minimum_required(VERSION 3.15)
project(CHIP8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output binary goes into build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source files from CHIP8 directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS CHIP8/*.cpp)

# Add include path for your own headers and SDL2
include_directories(
    CHIP8
    ${CMAKE_SOURCE_DIR}/SDL2/include
)

# Link SDL2 from local lib folder (adjust for x64/x86 as needed)
link_directories(${CMAKE_SOURCE_DIR}/SDL2/lib/x64)

# Create the executable
add_executable(chip8 ${SOURCES})

# Link SDL2
target_link_libraries(chip8 SDL2 SDL2main)

# Optional: for Windows, make sure subsystem is set
if (WIN32)
    set_target_properties(chip8 PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()