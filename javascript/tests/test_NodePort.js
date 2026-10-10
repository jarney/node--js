import {NodePort} from "../modules/NodePort.js"

var test = unitjs;

describe('NodePort', function(){
    it('test_NodePort_constructor', function(){
	var nodePort = new NodePort("int", "Integer", NodePort.ConnectionPolicy.Multiple);
	test.assert(nodePort != undefined, "test_NodePort_constructor ");
	test.assert(nodePort.getDataType() == "int", "test_NodePort_constructor DataType ==");
	test.assert(nodePort.getDescription() == "Integer", "test_NodePort_constructor Description ==");
	test.assert(nodePort.getConnectionPolicy() == NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy ==");
	test.assert(NodePort.ConnectionPolicy.One != NodePort.ConnectionPolicy.Multiple, "test_NodePort_constructor ConnectionPolicy not same");
    });
});
