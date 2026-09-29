import {UnitTests} from "./UnitTests.js"
import {NodeType} from "../modules/NodeType.js"
import {NodePort} from "../modules/NodePort.js"

UnitTests.TEST_CASE("test_NodeType_empty", function() {
    var nodeType = new NodeType();
    
    nodeType.setId("add");
    nodeType.setVisibility(NodeType.Visibility.PRIVATE);
    nodeType.setType(NodeType.Type.NATIVE);

    UnitTests.CHECK(nodeType.getVisibility() == NodeType.Visibility.PRIVATE, "visibility PRIVATE");
    UnitTests.CHECK(nodeType.getType() == NodeType.Type.NATIVE, "type NATIVE");

    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
    nodeType.setType(NodeType.Type.GRAPH);
    
    UnitTests.CHECK(nodeType.getVisibility() == NodeType.Visibility.PUBLIC, "visibility PUBLIC");
    UnitTests.CHECK(nodeType.getType() == NodeType.Type.GRAPH, "type GRAPH");
});

UnitTests.TEST_CASE("test_NodeType_some_ports", function () {
    var nodeType = new NodeType();
    nodeType.setId("add");
    nodeType.addInputPort("first", new NodePort("variable", "First Argument"));
    nodeType.addInputPort("second", new NodePort("variable", "Second Argument"));
    nodeType.addOutputPort("output", new NodePort("variable", "Result"));

    UnitTests.CHECK(nodeType.getInputPortCount() == 2, "nodeType.getInputPortCount() == 2");
    UnitTests.CHECK(nodeType.getOutputPortCount() == 1, "nodeType.getOutputPortCount() == 1");

    UnitTests.CHECK(nodeType.getInputPortByName("first").getDescription() == "First Argument", "nodeType.getInputPortByName(\"first\").getDescription() == \"First Argument\"");
    UnitTests.CHECK(nodeType.getInputPortName(0) == "first", "nodeType.getInputPortName(0) == \"first\"");
    UnitTests.CHECK(nodeType.getInputPortByIndex(0).getDescription() == "First Argument", "nodeType.getInputPortByIndex(0).getDescription() == \"First Argument\"");
    UnitTests.CHECK(nodeType.hasInputPort("first"), "nodeType.hasInputPort(\"first\")");
    
    UnitTests.CHECK(nodeType.getInputPortByName("second").getDescription() == "Second Argument", "nodeType.getInputPortByName(\"second\").getDescription() == \"Second Argument\"");
    UnitTests.CHECK(nodeType.getInputPortName(1) == "second", "nodeType.getInputPortName(1) == \"second\"");
    UnitTests.CHECK(nodeType.getInputPortByIndex(1).getDescription() == "Second Argument", "nodeType.getInputPortByIndex(1).getDescription() == \"Second Argument\"");

    UnitTests.CHECK(nodeType.getOutputPortByName("output").getDescription() == "Result", "nodeType.getOutputPortByName(\"output\").getDescription() == \"Result\"");
    UnitTests.CHECK(nodeType.getOutputPortName(0) == "output", "nodeType.getOutputPortName(0) == \"output\"");
    UnitTests.CHECK(nodeType.getOutputPortByIndex(0).getDescription() == "Result", "nodeType.getOutputPortByIndex(0).getDescription() == \"Result\"");
    UnitTests.CHECK(nodeType.hasOutputPort("output"), "nodeType.hasOutputPort(\"output\")");

    // Now, some edge cases:
    UnitTests.CHECK(nodeType.getInputPortByName("non-existent") == undefined, "nodeType.getInputPortByName(\"non-existent\") == undefined");
    UnitTests.CHECK(nodeType.getOutputPortByName("non-existent") == undefined, "nodeType.getOutputPortByName(\"non-existent\") == undefined");
    UnitTests.CHECK(!nodeType.hasInputPort("non-existent"), "!nodeType.hasInputPort(\"non-existent\")");
    UnitTests.CHECK(!nodeType.hasOutputPort("non-existent"), "!nodeType.hasOutputPort(\"non-existent\")");
    UnitTests.CHECK(nodeType.getInputPortName(99) == "", "nodeType.getInputPortName(99) == \"\"");
    UnitTests.CHECK(nodeType.getOutputPortName(99) == "", "nodeType.getOutputPortName(99) == \"\"");
    UnitTests.CHECK(nodeType.getInputPortByIndex(99) == undefined, "nodeType.getInputPortByIndex(99) == undefined");
    UnitTests.CHECK(nodeType.getOutputPortByIndex(1) == undefined, "nodeType.getOutputPortByIndex(1) == undefined");
});

UnitTests.TEST_CASE("test_NodeType_unique_input_ports", function() {
    var nodeType = new NodeType();
    var rc1 = nodeType.addInputPort("first", new NodePort("variable", "First Argument"));
    var rc2 = nodeType.addInputPort("first", new NodePort("variable", "Another"));

    UnitTests.CHECK(rc1 == true, "rc1");
    UnitTests.CHECK(rc2 == false, "rc2");

    UnitTests.CHECK(nodeType.getInputPortCount() == 1, "input count = 1");
    UnitTests.CHECK(nodeType.getOutputPortCount() == 0, "output count = 0");
    UnitTests.CHECK(nodeType.getInputPortByName("first").getDescription() == "First Argument", "test_NodeType_unique_input_ports first input wins");
});

UnitTests.TEST_CASE("test_NodeType_unique_output_ports", function() {
    var nodeType = new NodeType();
    var rc1 = nodeType.addOutputPort("first", new NodePort("variable", "First Argument"));
    var rc2 = nodeType.addOutputPort("first", new NodePort("variable", "Another"));

    UnitTests.CHECK(rc1 == true, "rc1");
    UnitTests.CHECK(rc2 == false, "rc2");

    UnitTests.CHECK(nodeType.getInputPortCount() == 0, "input count = 0");
    UnitTests.CHECK(nodeType.getOutputPortCount() == 1, "output count = 1");
    UnitTests.CHECK(nodeType.getOutputPortByName("first").getDescription() == "First Argument", "test_NodeType_unique_input_ports first input wins");
});

