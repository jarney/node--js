
import type {NodeId, EdgeId, PortId} from "./Definitions.ts"

/**
 * A DataType in NodeJS represents the type of data
 * flowing along a connection between nodes.  The physical
 * storage of data types is abstract and only identified by a
 * name.  The implementation of the physical means of connection
 * is governed by the specific nodes and the implementing platform.
 */
export class Edge {
  public fromNode: NodeId;
  public fromPort: PortId;
  public toNode: NodeId;
  public toPort: PortId;
  
  constructor(
    aFromNode: NodeId,
    aFromPort: PortId,
    aToNode: NodeId,
    aToPort: PortId
  ) {
    this.fromNode = aFromNode;
    this.fromPort = aFromPort;
    this.toNode = aToNode;
    this.toPort = aToPort;
  }
  
  public static equals(a: Edge, b: Edge) : boolean {
    return (a.fromNode == b.fromNode) &&
      (a.fromPort == b.fromPort) &&
      (a.toNode == b.toNode) &&
      (a.toPort == b.toPort);
  }
  
  getId(): EdgeId {
    const s = 
      this.fromNode + "-" + this.fromPort +
        "|" + 
        this.toNode + "-" + this.toPort;
    return s;
  }
}

