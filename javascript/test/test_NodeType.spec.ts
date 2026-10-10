import { describe, it, expect } from 'vitest'

import { NodeType } from '../src/NodeType.js'

describe('NodeType', () => {
  it('test_NodeType_empty', () => {
    var nodeType = new NodeType();
    
    nodeType.setId("add");
    nodeType.setVisibility(0); //NodeType.Visibility.PRIVATE);
    nodeType.setType(1); //NodeType.Type.NATIVE);

    expect(nodeType.getVisibility() == 0); //NodeType.Visibility.PRIVATE);
    expect(nodeType.getType() == 1); //NodeType.Type.NATIVE);

//    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
//    nodeType.setType(NodeType.Type.GRAPH);
//    
//    expect(nodeType.getVisibility() == NodeType.Visibility.PUBLIC, "visibility PUBLIC");
//    expect(nodeType.getType() == NodeType.Type.GRAPH, "type GRAPH");
  })
})

