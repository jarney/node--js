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

# Build the main package
pip install -r requirements.txt

# Build the developer stuff
pip install -r requirements-dev.txt

# Run the unit-tests
coverage run -m pytest
coverage html -d ${CMAKE_BINARY_DIR}/python/htmlcov

# Build the pydoc
sphinx-apidoc \
    -o docs2 \
    -H 'API Documentation' \
    node2

sphinx-build \
    -M html \
    ${PWD}/docs \
    ${CMAKE_BINARY_DIR}/python/docs/
