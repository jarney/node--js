import {UnitTests} from "./UnitTests.js"
import {NodePort} from "../modules/NodePort.js"

UnitTests.TEST_CASE("test_NodePort_constructor", function() {
    var nodePort = new NodePort("int", "Integer", NodePort.ConnectionPolicy.Multiple);
    UnitTests.CHECK(nodePort != undefined, "test_NodePort_constructor ");
    UnitTests.CHECK(nodePort.getDataType() == "int", "test_NodePort_constructor DataType ==");
    UnitTests.CHECK(nodePort.getDescription() == "Integer", "test_NodePort_constructor Description ==");
    UnitTests.CHECK(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy ==");
    UnitTests.CHECK(NodePort.ConnectionPolicy.One != NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy not same");
});

