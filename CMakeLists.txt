cmake_minimum_required(VERSION 3.5)
project(EscapeTheMaze C)

set(CMAKE_C_STANDARD 99)

# Add your main game source file
add_executable(${PROJECT_NAME} main.c)

# Include Raylib headers (adjust path if not installed via Homebrew)
include_directories(/opt/homebrew/opt/raylib/include)

# Link Raylib library (and required system libraries for Mac)
find_library(RAYLIB_LIB raylib HINTS /opt/homebrew/opt/raylib/lib)

# Fallback if Raylib is not found
if(NOT RAYLIB_LIB)
    message(FATAL_ERROR "Raylib library not found! Make sure it's installed via Homebrew.")
endif()

target_link_libraries(${PROJECT_NAME}
        ${RAYLIB_LIB}
        m                      # Math library
        pthread                # Thread support
        dl                     # Dynamic linking
        rt                     # Real-time extensions (on some Linux systems)
)
