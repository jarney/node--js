#!/bin/bash


VALGRIND_BIN=$1
TEST_FILE=$2
DATA_DIRECTORY=$3
MEMCHECK_LOG=$4

${VALGRIND_BIN} \
    --tool=memcheck \
    --leak-check=full \
    ${TEST_FILE} \
    --data-directory ${DATA_DIRECTORY} >${MEMCHECK_LOG} 2>&1

grep 'in use at exit: 0 bytes' ${MEMCHECK_LOG} >/dev/null 2>&1
let rc=$?

if [ $rc -ne 0 ] ; then
    echo "Error: Leak check found"
    cat ${MEMCHECK_LOG}
else
    exit 0
fi
