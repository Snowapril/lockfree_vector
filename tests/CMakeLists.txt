set(target Tests)
set(ROOT_DIR ${PROJECT_SOURCE_DIR})
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Sources
set(SRCS
    ${SRC_DIR}/perf_test.cpp
)

# Build executable
add_executable(${target} ${SRCS})

# Link libraries
target_link_libraries(${target}
    INTERFACE
	LockFree
)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)