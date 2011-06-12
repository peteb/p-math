# -*- cmake -*-

add_subdirectory(unittest)

add_library(utils
  ${UTILS_INCLUDE}/algorithm.h
  ${UTILS_INCLUDE}/vector.h
  ${UTILS_INCLUDE}/log.h

  log.cpp
)

include_directories(
  ${UTILS_INCLUDE}
)
