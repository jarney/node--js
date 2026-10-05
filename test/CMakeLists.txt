
find_package(Catch2 3 QUIET)

set(TEST_SOURCES
    main.cpp
    test_test.cpp
    test_NodePort.cpp
    test_DataType.cpp
    test_NodeType.cpp
    test_NodeModule.cpp
    test_NodeGraph.cpp
    test_Processor.cpp
    test_Edge.cpp
    test_ConnectionData.cpp
    test_Validator.cpp
    test_xml_Serializer.cpp
    test_xml_ModuleLoaderNodeJSPath.cpp
    test_xml_XmlNodeWrapper.cpp
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
        add_custom_target(code-coverage
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/code-coverage.sh ${PROJECT_BINARY_DIR}
        )
    else()
        message("Code coverage was asked for, but we don't have the dependencies to make it happen")
    endif()
endif()

if (BUILD_MEMCHECK)
    message("Enabling memory checking with valgrind")
    find_program(VALGRIND_BIN valgrind HINTS ${COMPILER_PATH})
    if (VALGRIND_BIN)
        add_custom_target(memcheck
	    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/memcheck.sh
	        ${VALGRIND_BIN}
		${PROJECT_BINARY_DIR}/test/nodejs_core_tests
		${PROJECT_SOURCE_DIR}/test-data/
		${PROJECT_BINARY_DIR}/memcheck.log
        )
    else()
        message("Valgrind not found, so we cannot do memory checks")
    endif()
endif()
