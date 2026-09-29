
find_package(Catch2 3 QUIET)

set(TEST_SOURCES
    Test_test.cpp
    Test_NodePort.cpp
    Test_DataType.cpp
    Test_NodeType.cpp
    Test_NodeModule.cpp
    )

add_executable(nodejs_tests ${TEST_SOURCES})

target_include_directories(nodejs_tests
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
  PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
)

target_link_libraries(nodejs_tests PRIVATE NodeJS Catch2::Catch2WithMain)

add_test(
  NAME test_nodes
  COMMAND
    $<TARGET_FILE:nodejs_tests>
)

#########################
# Code coverage report
#########################
if (BUILD_CODE_COVERAGE)
    add_custom_target(gcov-report
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/gcov-report.sh ${PROJECT_BINARY_DIR}
    )
endif()
