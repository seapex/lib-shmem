# define the executable target
set (LIB_TARGET "shmem")
#message("LIB_TARGET: ${LIB_TARGET}")

message("_ARCH_TYPE: ${_ARCH_TYPE}")

add_sources(share_mem.cpp)

set ( SRC_LIST ${SRC_LIST} ${COMMON_SRC})
message("SRC_LIST=${SRC_LIST}")

#set (EXTRA_LIBS ${EXTRA_LIBS} pthread dl)
# add the library
add_library (${LIB_TARGET} STATIC ${SRC_LIST})
target_link_libraries (${LIB_TARGET} ${EXTRA_LIBS})

# Set the GCC Additional flags to use when compiling this target's sources
# -O0 -O1 -O2 -O3 ...
set_property(TARGET ${LIB_TARGET} PROPERTY COMPILE_FLAGS -O2)

# Set the GCC Additional flags to use when linking this target
# -shared-libgcc -static-libstdc++ -static-libgcc ...
set_property(TARGET ${LIB_TARGET} PROPERTY LINK_FLAGS -static-libgcc)

# specifies the directory into which runtime target files should be built
#set_property( TARGET ${LIB_TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY
#              ${PROJECT_BINARY_DIR}/bin )

# add the install targets
set(BIN2DES /home/works/emctrl/main/lib)
install (TARGETS ${LIB_TARGET} DESTINATION ${BIN2DES})
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/share_mem.h DESTINATION ${BIN2DES})
