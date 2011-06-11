find_package(GTest REQUIRED)

include_directories(
  ${LIBRARY_INCLUDE}
  ${GTEST_INCLUDE_DIR}
)

add_executable(unittest_utils EXCLUDE_FROM_ALL
  test_vector.cpp  
)

target_link_libraries(unittest_utils
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
)

add_dependencies(unittest unittest_utils)
