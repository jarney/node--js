import {test_DataType} from "./test_DataType.js";
import {test_NodePort} from "./test_NodePort.js";
import {test_NodeType} from "./test_NodeType.js";
import {TEST_RUN} from "./CHECK.js";

function UnitTests() {
}

UnitTests.prototype.run = function() {
    var output = document.getElementById("console");
    output.textContent = "";
    TEST_RUN();
};

export { UnitTests };
