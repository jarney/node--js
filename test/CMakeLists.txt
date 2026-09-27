# TODO: Eventually build the unit tests here...

find_package(Catch2 3 QUIET)

set(TEST_SOURCES
    Test_test.cpp
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

