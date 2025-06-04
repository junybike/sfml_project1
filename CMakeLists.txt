cmake_minimum_required(VERSION 3.10)
project(StickmanGame)

set(CMAKE_CXX_STANDARD 17)

# Set source and include directories
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(game ${SOURCES})

# Link SFML
target_link_libraries(game sfml-graphics sfml-window sfml-system)

# Copy assets to the build directory
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
