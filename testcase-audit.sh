
if [ $# -ne 1 ] ; then
    echo "Usage: testcase-audit.sh <directory>"
    exit 1
fi

CMAKE_PROJECT_DIRECTORY=$1
#echo "Directory:"
#echo ${CMAKE_PROJECT_DIRECTORY}
#exit 0

# All cpp test cases
cat ${CMAKE_PROJECT_DIRECTORY}/test/*.cpp | \
    grep 'TEST_CASE' | \
    grep -v '_cpp_specific' | \
    sed 's/TEST_CASE("//g' | \
    sed 's/".*//g'| \
    awk '{print($0 "" );}' | \
    sort >${CMAKE_PROJECT_DIRECTORY}/build/tests-cpp.txt

# All python test cases
cat ${CMAKE_PROJECT_DIRECTORY}/python/test/*.py | \
    grep 'def test_' | \
    grep -v '_py_specific' | \
    sed 's/#\?def test_/test_/g' | \
    sed 's/():.*//g' | \
    awk '{print($1 "");}' | \
    sort > ${CMAKE_PROJECT_DIRECTORY}/build/tests-py.txt

# All JS test cases
cat ${CMAKE_PROJECT_DIRECTORY}/javascript/tests/*.js | \
    grep TEST_CASE | \
    grep 'test_' | \
    grep -v '_js_specific' | \
    sed 's/.*TEST_CASE("//g' | \
    sed 's/".*//g' | \
    awk '{print($1 "");}' | \
    sort > ${CMAKE_PROJECT_DIRECTORY}/build/tests-js.txt

# Differences to Python test-cases.
diff ${CMAKE_PROJECT_DIRECTORY}/build/tests-cpp.txt \
     ${CMAKE_PROJECT_DIRECTORY}/build/tests-py.txt

# Differences to JS test-cases.
#diff ${CMAKE_PROJECT_DIRECTORY}/build/tests-cpp.txt \
#     ${CMAKE_PROJECT_DIRECTORY}/build/tests-js.txt
