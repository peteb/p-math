# -*- cmake -*-

cmake_minimum_required(VERSION 2.8)

project(p)
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_INCLUDE ${PROJECT_ROOT_DIR}/include)

if(NOT TARGET)
  message(FATAL_ERROR "no target set. verify no target by setting target to 'tests'.")
endif(NOT TARGET)

# each platform can have unittests
# there are generic unittests that always exist
# cmake -DTARGET=iOS    -> binaries for iOS, resources, tests, generic tests

add_custom_target(unittest)

set(UTILS_INCLUDE ${PROJECT_ROOT_DIR}/include/utils)
set(TARGET_CONF_PATH ${PROJECT_ROOT_DIR}/build/${TARGET}/CMakeLists.txt)

if(NOT EXISTS TARGET_CONF_PATH)
#  message(FATAL_ERROR "invalid target: ${TARGET}.")
endif(NOT EXISTS TARGET_CONF_PATH)

add_subdirectory(${PROJECT_ROOT_DIR}/src/utils CMakeFiles/utils)
#include(${PROJECT_ROOT_DIR}/src/utils/CMakeLists.txt)
#include(TARGET_CONF_PATH)