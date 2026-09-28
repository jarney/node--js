#!/bin/bash

if [ $# -eq 1 ] ; then
    CMAKE_BUILD_DIRECTORY=$1
else
    CMAKE_BUILD_DIRECTORY=build
fi

mkdir -p ${CMAKE_BUILD_DIRECTORY}/gcov

gcovr \
     --exclude 'test/.*.cpp' \
     --html ${CMAKE_BUILD_DIRECTORY}/gcov/report.html \
     --html-details
