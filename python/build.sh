#!/bin/bash

if [ $# -ne 2 ] ; then
    echo "Usage: buil.sh cmake-binary-dir cmake-source-dir"
    exit 1
fi
CMAKE_BINARY_DIR=$1
CMAKE_SOURCE_DIR=$2

exit 0

mkdir -p ${CMAKE_BINARY_DIR}/python
if [ ! -d ${CMAKE_BINARY_DIR}/python/venv ] ; then
    python3 -m venv ${CMAKE_BINARY_DIR}/python/venv
fi

. ${CMAKE_BINARY_DIR}/python/venv/bin/activate

# Build the main package
#pip install -r requirements.txt

# Build the developer stuff
#pip install -r requirements-dev.txt

# Run the unit-tests
coverage run -m pytest --data-directory=${CMAKE_SOURCE_DIR}/test-data -s
coverage html \
	 --omit='test/*' \
	 -d ${CMAKE_BINARY_DIR}/python/htmlcov

# Build the pydoc
if [ 1 -eq 0 ] ; then
    sphinx-apidoc \
	-o docs/api \
	-H 'API Documentation' \
	node2
    
    sphinx-build \
	-M html \
	${PWD}/docs \
	${CMAKE_BINARY_DIR}/python/docs/
fi
