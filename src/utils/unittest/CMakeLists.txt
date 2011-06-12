find_package(GTest REQUIRED)

add_executable(unittest_utils EXCLUDE_FROM_ALL
  test_vector.cpp  
)

include_directories(
  ${LIBRARY_INCLUDE}
  ${GTEST_INCLUDE_DIR}
)

target_link_libraries(unittest_utils
  ${GTEST_MAIN_LIBRARY}
  ${GTEST_LIBRARY}
  utils
)

add_dependencies(unittest unittest_utils)
