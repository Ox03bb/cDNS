cmake_minimum_required(VERSION 3.10)
project(cDNS C)

# ---- Compiler settings ----
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable extra warnings in Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -g")

# ---- Paths ----
set(SRC_DIR     ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/includes)
set(VENDOR_DIR  ${PROJECT_SOURCE_DIR}/vendor)

# ---- Sources ----
# List sources explicitly for reliability
set(CDNS_SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/protocol/parser.c
    ${SRC_DIR}/protocol/free.c
)

# ---- tomlc99 library ----
add_library(tomlc99 STATIC
    ${VENDOR_DIR}/tomlc99/toml.c
)
target_include_directories(tomlc99 PUBLIC
    ${VENDOR_DIR}/tomlc99
)

# ---- Executable ----
add_executable(cDNS ${CDNS_SOURCES})
target_include_directories(cDNS PRIVATE
    ${INCLUDE_DIR}
	${INCLUDE_DIR}/protocol
)
target_link_libraries(cDNS PRIVATE
    tomlc99
)

# ---- Convenience targets ----
add_custom_target(build_script
    COMMAND ${CMAKE_COMMAND} -E echo "Run ./scripts/build.sh to build manually"
)
add_custom_target(format_script
    COMMAND ${CMAKE_COMMAND} -E echo "Run ./scripts/format.sh to format code"
)
