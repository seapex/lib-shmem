# define the executable target
set (LIB_TARGET "shmem61850")
message("LIB_TARGET: ${LIB_TARGET}")

message("_ARCH_TYPE: ${_ARCH_TYPE}")

add_sources(shmem_srv61850.cpp shmem_sv_61850.cpp)
include_directories (../common ./)
include_directories (../../../emctrl/common/config)
#set(TEST_LIBS "")
#aux_source_directory(../common COMMON_SRC)

set ( SRC_LIST ${SRC_LIST} ${COMMON_SRC})
message("SRC_LIST=${SRC_LIST}")

set (EXTRA_LIBS ${EXTRA_LIBS} shmem)
message("EXTRA_LIBS=${EXTRA_LIBS}")
# add the executable
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
set(INSTALL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shmem_sv_61850.h)
set(INSTALL_SRC ${INSTALL_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/shmem_srv61850.h)
set(SRC2DES /home/works/emctrl/main/lib)
set(BIN2DES /home/works/emctrl/main/lib)
install (TARGETS ${LIB_TARGET} DESTINATION ${BIN2DES})
install (FILES ${INSTALL_SRC} DESTINATION ${SRC2DES})
