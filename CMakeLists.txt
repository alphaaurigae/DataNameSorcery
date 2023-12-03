cmake_minimum_required(VERSION 3.12)
project(datanamesorcery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
 -fsanitize=undefined -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=vla-bound \
 -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size \
 -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool -fsanitize=enum \
 -fsanitize=vptr -fsanitize=pointer-overflow -fsanitize=builtin -fsanitize=thread")  # -fsanitize=leak #   # -fsanitize=pointer-compare  -fsanitize=address

# Add include directories
include_directories(/usr/include)

# Add source files
set(SRCS datanamesorcery.cpp)

# Build executable
add_executable(datanamesorcery ${SRCS})

# Link libraries
target_link_libraries(datanamesorcery uv PocoFoundation PocoNet xerces-c)

# Set output directory
set(BINDIR bin)
set_target_properties(datanamesorcery PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${BINDIR}
)

# Create a custom target for moving the executable to bin directory
add_custom_target(move
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:datanamesorcery> ${CMAKE_CURRENT_BINARY_DIR}/${BINDIR}
    COMMENT "Moving executable to bin directory"
)

# Create a custom target for cleaning
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/${BINDIR}
    COMMENT "Cleaning build files"
)

# Add dependencies between targets
add_dependencies(move datanamesorcery)
add_dependencies(clean_all datanamesorcery)

# Run cmake with the --debug-output option for verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)