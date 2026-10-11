import {GroupId, NodeId} from "./Definitions.ts"

/**
 * A DataType in NodeJS represents the type of data
 * flowing along a connection between nodes.  The physical
 * storage of data types is abstract and only identified by a
 * name.  The implementation of the physical means of connection
 * is governed by the specific nodes and the implementing platform.
 */
export class Group {
    mNodes: { [key: NodeId ] : NodeId };

    constructor() {
        this.mNodes = {}
    }
    public addNode(aNodeId: NodeId) {
        this.mNodes[aNodeId] = aNodeId;
    }
    public removeNode(aNodeId: NodeId) {
        delete this.mNodes[aNodeId];
    }
    public getNodes(): [ NodeId ] {
        var nodes: [ NodeId ] = [];
	for (const node in this.mNodes) {
	    nodes.push(node);
	}
	return nodes;
    }
    public contains(aNodeId: NodeId): boolean {
        return aNodeId in this.mNodes;
    }
}
