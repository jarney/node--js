#!/bin/bash

if [ $# -ne 1 ] ; then
    echo "Supply output directory in argv[1]"
    exit 0
fi
CMAKE_BINARY_DIR=$1

mkdir -p ${CMAKE_BINARY_DIR}/python
if [ ! -d ${CMAKE_BINARY_DIR}/python/venv ] ; then
    python3 -m venv ${CMAKE_BINARY_DIR}/python/venv
fi

. ${CMAKE_BINARY_DIR}/python/venv/bin/activate


pip install -r requirements.txt
coverage run -m pytest
coverage html -d ${CMAKE_BINARY_DIR}/python/htmlcov
