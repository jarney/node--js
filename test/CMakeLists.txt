
find_package(Catch2 3 QUIET)

set(TEST_SOURCES
    main.cpp
    Test_test.cpp
    Test_NodePort.cpp
    Test_DataType.cpp
    Test_NodeType.cpp
    Test_NodeModule.cpp
    Test_NodeGraph.cpp
    Test_ConnectionData.cpp
    Test_xml_Serializer.cpp
    )

add_executable(nodejs_core_tests ${TEST_SOURCES})

target_include_directories(nodejs_core_tests
  PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/test/>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
)

target_link_libraries(nodejs_core_tests PRIVATE NodeJS Catch2::Catch2)

add_test(
  NAME nodejs_core_tests
  COMMAND
    $<TARGET_FILE:nodejs_core_tests> --data-directory ${PROJECT_SOURCE_DIR}/test-data/
)

#########################
# Code coverage report
#########################
if (BUILD_CODE_COVERAGE)
    find_program(GCOV_BIN gcov HINTS ${COMPILER_PATH})
    find_program(GCOVR_BIN gcovr HINTS ${COMPILER_PATH})
    if (NOT GCOV_BIN)
        message("Code coverage was asked for, but we could not find gcov")
    endif()
    if (NOT GCOVR_BIN)
        message("Code coverage was asked for, but we could not find gcovr")
    endif()

    if (GCOV_BIN AND GCOVR_BIN)
        add_custom_target(gcov-report
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/gcov-report.sh ${PROJECT_BINARY_DIR}
        )
    else()
        message("Code coverage was asked for, but we don't have the dependencies to make it happen")
    endif()
endif()

