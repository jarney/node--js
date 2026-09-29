var tests = [];

function TEST_CASE(name, fn) {
    tests.push({name: name, fn: fn});
};

function CHECK(condition, msg) {
    var output = document.getElementById("console");
    if (condition) {
	msg += " OK";
    }
    else {
	msg += " FAIL";
    }
    output.textContent = output.textContent + "        " + msg + "\n";
    console.log(msg);
}

function TEST_BEGIN(msg) {
    var output = document.getElementById("console");
    console.log("TEST: " + msg);
    output.textContent = output.textContent + msg + "\n";
}

function TEST_RUN() {
    tests.forEach(function(test) {
	TEST_BEGIN(test.name);
	test.fn();
    });
}

export {CHECK, TEST_CASE, TEST_BEGIN, TEST_RUN};
