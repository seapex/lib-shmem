# define the executable target
set (EXE_TARGET "tst_shmem")

add_sources(main.cpp parse_optn_shmem.cpp)
include_directories (../common ../61850)
set(COMMON_PATH "../../../emctrl/common")
include_directories (${COMMON_PATH}/config)
include(${COMMON_PATH}/parse_optn/CMakeLists.txt)

add_library(shmemtst STATIC IMPORTED)
set_property(TARGET shmemtst PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/common/libshmem.a)
add_library(shmem61850tst STATIC IMPORTED)
set_property(TARGET shmem61850tst PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/61850/libshmem61850.a)
set (EXTRA_LIBS ${EXTRA_LIBS} shmem61850tst shmemtst) # Notice! The order here is very important

# add the executable or library target
add_executable (${EXE_TARGET} ${SRC_LIST})
target_link_libraries (${EXE_TARGET} ${EXTRA_LIBS})

# Set the GCC Additional flags to use when compiling this target's sources
# -O0 -O1 -O2 -O3 ...
set_property(TARGET ${EXE_TARGET} PROPERTY COMPILE_FLAGS -O2)

# Set the GCC Additional flags to use when linking this target
# -shared-libgcc -static-libstdc++ -static-libgcc ...
set_property(TARGET ${EXE_TARGET} PROPERTY LINK_FLAGS -static-libgcc)

# specifies the directory into which runtime target files should be built
#set_property( TARGET ${LIB_TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY
#              ${PROJECT_BINARY_DIR}/bin )

# add the install targets

