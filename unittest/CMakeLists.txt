# -*- cmake -*-

cmake_minimum_required(VERSION 2.8)

project(p)
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_INCLUDE ${PROJECT_ROOT_DIR})



add_custom_target(run-unittest 
  ./run-unittest.sh ${PROJECT_ROOT_DIR}
)

add_dependencies(run-unittest unittest)

add_custom_target(clean-gen
	rm -rf CMakeFiles CMakeCache.txt Makefile ../bin 
)


# some compiler flags. would be better to have them somewhere else..
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
  set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -Wall -Wextra -g")
endif(CMAKE_COMPILER_IS_GNUCXX)



find_package(GTest REQUIRED)

add_executable(unittest EXCLUDE_FROM_ALL
  vector_test.cpp  
)

include_directories(
  ${LIBRARY_INCLUDE}
  ${GTEST_INCLUDE_DIR}
)

target_link_libraries(unittest
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
)
