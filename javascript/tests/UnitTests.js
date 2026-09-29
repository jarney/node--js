var UnitTests = {};
var tests = [];

UnitTests.TEST_CASE = function(name, fn) {
    tests.push({name: name, fn: fn});
};

UnitTests.CHECK = function(condition, msg) {
    var output = document.getElementById("console");
    if (condition) {
	msg += " OK";
    }
    else {
	msg += " FAIL";
    }
    output.textContent = output.textContent + "        " + msg + "\n";
    console.log(msg);
};

UnitTests.TEST_BEGIN = function(msg) {
    var output = document.getElementById("console");
    console.log("TEST: " + msg);
    output.textContent = output.textContent + msg + "\n";
};

UnitTests.RUN = function() {
    var output = document.getElementById("console");
    output.textContent = "";
    tests.forEach(function(test) {
	UnitTests.TEST_BEGIN(test.name);
	test.fn();
    });
};

export { UnitTests };
