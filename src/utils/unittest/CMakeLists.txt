find_package(GTest REQUIRED)

add_executable(utils_unittest EXCLUDE_FROM_ALL
  vector_test.cpp  
)

include_directories(
  ${LIBRARY_INCLUDE}
  ${GTEST_INCLUDE_DIR}
)

target_link_libraries(utils_unittest
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
  utils
)

add_dependencies(unittest utils_unittest)
