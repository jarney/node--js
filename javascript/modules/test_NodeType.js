import {CHECK, TEST_CASE} from "./CHECK.js"
import {NodeType} from "./NodeType.js"
import {NodePort} from "./NodePort.js"

TEST_CASE("test_NodeType_empty", function() {
    var nodeType = new NodeType();
    
    nodeType.setId("add");
    nodeType.setVisibility(NodeType.Visibility.PRIVATE);
    nodeType.setType(NodeType.Type.NATIVE);

    CHECK(nodeType.getVisibility() == NodeType.Visibility.PRIVATE, "visibility PRIVATE");
    CHECK(nodeType.getType() == NodeType.Type.NATIVE, "type NATIVE");

    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
    nodeType.setType(NodeType.Type.GRAPH);
    
    CHECK(nodeType.getVisibility() == NodeType.Visibility.PUBLIC, "visibility PUBLIC");
    CHECK(nodeType.getType() == NodeType.Type.GRAPH, "type GRAPH");
});

TEST_CASE("test_NodeType_some_ports", function () {
    var nodeType = new NodeType();
    nodeType.setId("add");
    nodeType.addInputPort("first", new NodePort("variable", "First Argument"));
    nodeType.addInputPort("second", new NodePort("variable", "Second Argument"));
    nodeType.addOutputPort("output", new NodePort("variable", "Result"));

    CHECK(nodeType.getInputPortCount() == 2, "nodeType.getInputPortCount() == 2");
    CHECK(nodeType.getOutputPortCount() == 1, "nodeType.getOutputPortCount() == 1");

    CHECK(nodeType.getInputPortByName("first").getDescription() == "First Argument", "nodeType.getInputPortByName(\"first\").getDescription() == \"First Argument\"");
    CHECK(nodeType.getInputPortName(0) == "first", "nodeType.getInputPortName(0) == \"first\"");
    CHECK(nodeType.getInputPortByIndex(0).getDescription() == "First Argument", "nodeType.getInputPortByIndex(0).getDescription() == \"First Argument\"");
    CHECK(nodeType.hasInputPort("first"), "nodeType.hasInputPort(\"first\")");
    
    CHECK(nodeType.getInputPortByName("second").getDescription() == "Second Argument", "nodeType.getInputPortByName(\"second\").getDescription() == \"Second Argument\"");
    CHECK(nodeType.getInputPortName(1) == "second", "nodeType.getInputPortName(1) == \"second\"");
    CHECK(nodeType.getInputPortByIndex(1).getDescription() == "Second Argument", "nodeType.getInputPortByIndex(1).getDescription() == \"Second Argument\"");

    CHECK(nodeType.getOutputPortByName("output").getDescription() == "Result", "nodeType.getOutputPortByName(\"output\").getDescription() == \"Result\"");
    CHECK(nodeType.getOutputPortName(0) == "output", "nodeType.getOutputPortName(0) == \"output\"");
    CHECK(nodeType.getOutputPortByIndex(0).getDescription() == "Result", "nodeType.getOutputPortByIndex(0).getDescription() == \"Result\"");
    CHECK(nodeType.hasOutputPort("output"), "nodeType.hasOutputPort(\"output\")");

    // Now, some edge cases:
    CHECK(nodeType.getInputPortByName("non-existent") == undefined, "nodeType.getInputPortByName(\"non-existent\") == undefined");
    CHECK(nodeType.getOutputPortByName("non-existent") == undefined, "nodeType.getOutputPortByName(\"non-existent\") == undefined");
    CHECK(!nodeType.hasInputPort("non-existent"), "!nodeType.hasInputPort(\"non-existent\")");
    CHECK(!nodeType.hasOutputPort("non-existent"), "!nodeType.hasOutputPort(\"non-existent\")");
    CHECK(nodeType.getInputPortName(99) == "", "nodeType.getInputPortName(99) == \"\"");
    CHECK(nodeType.getOutputPortName(99) == "", "nodeType.getOutputPortName(99) == \"\"");
    CHECK(nodeType.getInputPortByIndex(99) == undefined, "nodeType.getInputPortByIndex(99) == undefined");
    CHECK(nodeType.getOutputPortByIndex(1) == undefined, "nodeType.getOutputPortByIndex(1) == undefined");
});

TEST_CASE("test_NodeType_unique_input_ports", function() {
    var nodeType = new NodeType();
    var rc1 = nodeType.addInputPort("first", new NodePort("variable", "First Argument"));
    var rc2 = nodeType.addInputPort("first", new NodePort("variable", "Another"));

    CHECK(rc1 == true, "rc1");
    CHECK(rc2 == false, "rc2");

    CHECK(nodeType.getInputPortCount() == 1, "input count = 1");
    CHECK(nodeType.getOutputPortCount() == 0, "output count = 0");
    CHECK(nodeType.getInputPortByName("first").getDescription() == "First Argument", "test_NodeType_unique_input_ports first input wins");
});

TEST_CASE("test_NodeType_unique_output_ports", function() {
    var nodeType = new NodeType();
    var rc1 = nodeType.addOutputPort("first", new NodePort("variable", "First Argument"));
    var rc2 = nodeType.addOutputPort("first", new NodePort("variable", "Another"));

    CHECK(rc1 == true, "rc1");
    CHECK(rc2 == false, "rc2");

    CHECK(nodeType.getInputPortCount() == 0, "input count = 0");
    CHECK(nodeType.getOutputPortCount() == 1, "output count = 1");
    CHECK(nodeType.getOutputPortByName("first").getDescription() == "First Argument", "test_NodeType_unique_input_ports first input wins");
});

function test_NodeType() {}
export {test_NodeType};
