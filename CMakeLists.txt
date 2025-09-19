cmake_minimum_required(VERSION 3.10)
project( cDNS )

find_package(OpenSSL REQUIRED)

# Enable sanitizers in Debug mode
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -g -O1")

# Source files
set(SOURCES
    src/*
)

# Include directories
include_directories(
    includes
)

add_executable(cDNS ${SOURCES})

target_link_libraries(cDNS PRIVATE)