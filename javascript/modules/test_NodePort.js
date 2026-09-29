import {CHECK, TEST_CASE} from "./CHECK.js"
import {NodePort} from "./NodePort.js"

TEST_CASE("test_NodePort_constructor", function() {
    var nodePort = new NodePort("int", "Integer", NodePort.ConnectionPolicy.Multiple);
    CHECK(nodePort != undefined, "test_NodePort_constructor ");
    CHECK(nodePort.getDataType() == "int", "test_NodePort_constructor DataType ==");
    CHECK(nodePort.getDescription() == "Integer", "test_NodePort_constructor Description ==");
    CHECK(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy ==");
    CHECK(NodePort.ConnectionPolicy.One != NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy not same");
});

function test_NodePort() {}
export {test_NodePort};
