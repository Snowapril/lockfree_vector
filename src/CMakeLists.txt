# Target name
set(target LockFree)

# Build header-only library
add_library(${target} INTERFACE ${HEADERS})

#Include directories
target_include_directories(${target} INTERFACE)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)