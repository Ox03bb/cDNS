
cmake_minimum_required(VERSION 3.10)
project(cDNS C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/includes)

# Source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

# Add executable
add_executable(cDNS ${SOURCES})

# Custom build and format scripts (hints)
add_custom_target(build_script
	COMMAND ${CMAKE_COMMAND} -E echo "Run ./scripts/build.sh to build manually"
)
add_custom_target(format_script
	COMMAND ${CMAKE_COMMAND} -E echo "Run ./scripts/format.sh to format code"
)
