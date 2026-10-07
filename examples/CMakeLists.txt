set(CALC_SOURCE_FILES
  passthrough.cpp
)

set(CALC_HEADER_FILES

)

add_executable(passthrough
  ${CALC_SOURCE_FILES}
  ${CALC_HEADER_FILES}
)

target_link_libraries(passthrough NodeJS)

