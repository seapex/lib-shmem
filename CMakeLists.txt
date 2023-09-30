cmake_minimum_required (VERSION 2.8)
project (ShareMemory)

# The version number.
set (_VERSION_MAJOR 1)
set (_VERSION_MINOR 0)
set (_VERSION_PATCH 7)

if(CMAKE_C_COMPILER MATCHES "arm")
	set (_ARCH_TYPE ARM)
else()
	set (_ARCH_TYPE X86)
endif()
add_definitions(-D${_ARCH_TYPE})

macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRC_LIST "${_relPath}/${_src}")
        else()
            list (APPEND SRC_LIST "${_src}")
        endif()
    endforeach()
endmacro()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

# add the 61850/ common/ test/ to the search path for src files
add_subdirectory (common)
add_subdirectory (61850)
add_subdirectory (test)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${_VERSION_PATCH}")
include (CPack)

