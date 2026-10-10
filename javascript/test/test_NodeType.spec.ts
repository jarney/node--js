import { describe, it, expect } from 'vitest'

import { NodeType } from '../src/NodeType.ts'

describe('NodeType', () => {
  it('test_NodeType_empty', () => {

    var nodeType = new NodeType();
    
    nodeType.setId("add");
    nodeType.setVisibility(NodeType.Visibility.PRIVATE);
    nodeType.setType(NodeType.Type.NATIVE);

    expect(nodeType.getVisibility()).to.equal(NodeType.Visibility.PRIVATE);
    expect(nodeType.getType()).to.equal(NodeType.Type.NATIVE);

    nodeType.setVisibility(NodeType.Visibility.PUBLIC);
    nodeType.setType(NodeType.Type.GRAPH);

    expect(nodeType.getVisibility()).to.equal(NodeType.Visibility.PUBLIC);
    expect(nodeType.getType()).to.equal(NodeType.Type.GRAPH);

  });
});
