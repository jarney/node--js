set(CALC_SOURCE_FILES
  example.cpp
)

set(CALC_HEADER_FILES
  example.hpp
)

add_executable(example
  ${CALC_SOURCE_FILES}
  ${CALC_HEADER_FILES}
)

target_link_libraries(example NodeJS)

