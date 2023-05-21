# define the executable target
set (LIB_TARGET "shmem")

add_sources(share_mem.cpp)

# add the executable or library target
add_library (${LIB_TARGET} STATIC ${SRC_LIST})
# target_link_libraries (${LIB_TARGET} ${EXTRA_LIBS})

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
set(INSTALL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/share_mem.h)
set(INSTALL_DES /home/works/emctrl/1.lib)

install (TARGETS ${LIB_TARGET} DESTINATION ${INSTALL_DES})
install (FILES ${INSTALL_SRC} DESTINATION ${INSTALL_DES})

set(RELEASE4THIRD ${CMAKE_SOURCE_DIR}/doc/release)
install (TARGETS ${LIB_TARGET} DESTINATION ${RELEASE4THIRD})
install (FILES ${INSTALL_SRC} DESTINATION ${RELEASE4THIRD})
