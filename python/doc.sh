CMAKE_BINARY_DIR=../build

rm -rf docs/api
mkdir -p docs/api

sphinx-apidoc \
    -o docs/api \
    -H 'API Documentation' \
    node2

sphinx-build -M html ${PWD}/docs ${CMAKE_BINARY_DIR}/python/docs/
