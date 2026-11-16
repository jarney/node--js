#########################################
# Validator utility to validate xml files
# and make sure they comply with the
# official schema.
#########################################
set(VALIDATOR_SOURCE_FILES
    val.cpp
)
set(VALIDATOR_HEADER_FILES

)

add_executable(validator
  ${VALIDATOR_SOURCE_FILES}
  ${VALIDATOR_HEADER_FILES}
)

target_link_libraries(validator NodeJS)

#########################################
# This is a simple 'loopback'
# kind-of test which reads a module,
# parses it for validity and writes
# it back out again.
#########################################
set(PASSTHROUGH_SOURCE_FILES
  passthrough.cpp
)

set(PASSTHROUGH_HEADER_FILES

)

add_executable(passthrough
  ${PASSTHROUGH_SOURCE_FILES}
  ${PASSTHROUGH_HEADER_FILES}
)

target_link_libraries(passthrough NodeJS)

#########################################
# This is just a core 'process' utility
# which reads a graph, parses it for
# validity, and then executes the 'main'
# graph on it.  There are a number of
# practical examples that can be done with
# this alone such as generating makfiles,
# openscad, and even lisp code.
#########################################

set(PROCESS_SOURCE_FILES
  process.cpp
)

set(PROCESS_HEADER_FILES

)

add_executable(process
  ${PROCESS_SOURCE_FILES}
)
target_include_directories(process PUBLIC ${Boost_INCLUDE_DIR} )

target_link_libraries(process NodeJS ${Boost_LIBRARIES})

#########################################
# Generated header.
#########################################
add_executable(gen)
add_custom_command(
  OUTPUT generated.hpp
  COMMAND xsltproc ${CMAKE_CURRENT_SOURCE_DIR}/schema-to-hpp.xsl ${CMAKE_CURRENT_SOURCE_DIR}/org.ensor.nodejs.xsd > ${CMAKE_CURRENT_SOURCE_DIR}/generated.hpp
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/schema-to-hpp.xsl ${CMAKE_CURRENT_SOURCE_DIR}/org.ensor.nodejs.xsd
  VERBATIM
)
target_sources(gen PRIVATE gen.cpp generated.hpp)
