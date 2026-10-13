
import type { NodeTypeId, NodeId, EdgeId, PortId} from "./Definitions.ts"
import { NamedPorts } from "./NamedPorts.ts";
import { ConnectionData } from "./ConnectionData.ts";
import { Metadata } from "./Metadata.ts";

/**
 * This class represents a type of node in a node program.
 * Node types are characterized by a unique identifier.
 * Node types carry a visibility field indicating whether
 * nodes outside this package are allowed to access it or not.
 * In addition, a node may be marked as 'native', indicating that
 * its implementation is provided by the underlying
 * runtime, or as 'graph' indicating that the implementation
 * is provided by a graph of other nodes.
 *
 * In addition, each node declares input and output ports
 * which may carry data.  Each input and output port are
 * associated with a data type.
 */

export class NodeType {
    mId: NodeTypeId;
    mVisibility: NodeType.Visibility;
    mType: NodeType.Type;
    mInputs: NamedPorts;
    mOutputs: NamedPorts;
    mDefaultNodeData: ConnectionData;
    mMetadata: Metadata;

    constructor() {
        this.mId = "";
        this.mVisibility = NodeType.Visibility.PRIVATE;
        this.mType = NodeType.Type.NATIVE;
        this.mInputs = new NamedPorts();
        this.mOutputs = new NamedPorts();
	this.mDefaultNodeData = new ConnectionData();
	this.mMetadata = new Metadata();
    }

    public getId(): NodeTypeId {
        return this.mId;
    }

    public setId(aId: NodeTypeId) {
        this.mId = aId;
    }

    public setVisibility(aVisibility: NodeType.Visibility) {
        this.mVisibility = aVisibility;
    }
    public getVisibility() : NodeType.Visibility {
        return this.mVisibility;
    }
    public setType(aType: NodeType.Type) {
        this.mType = aType;
    }
    public getType() : NodeType.Type {
        return this.mType;
    }
    public getInputs() : NamedPorts {
        return this.mInputs;
    }
    public getOutputs() : NamedPorts {
        return this.mOutputs;
    }
    public getDefaultNodeData(): ConnectionData {
        return this.mDefaultNodeData;
    }
    public setDefaultNodeData(aConnectionData: ConnectionData) {
        this.mDefaultNodeData = new ConnectionData();
	this.mDefaultNodeData.copy(aConnectionData);
    }
    public getMetadata() : Metadata {
        return this.mMetadata;
    }
}

export namespace NodeType {
  /**
   * This enum represents the visibility of a node type.
   * This determines whether graphs outside this package are
   * permitted to access it.
   */
  export enum Visibility {
    /**
     * This indicates that a node type is public and may
     * be used in any graph context.
     */
    PUBLIC = 0,
    /**
     * This indicates that a node type is private
     * and may be used only in the context of the node module
     * where it was declared.
     */
    PRIVATE = 1
  };
  
  /**
   * This enum represents the declaration of how a node
   * is implemented.
   */
  export enum Type {
    /**
     * This indicates that the node's implementation is
     * provided by the underlying runtime.
     */
    NATIVE = 0,
    /**
     * This indicates that the node's implementation is
     * provided by a node graph consisting of other node types.
     */
    GRAPH = 1,

    /**
     * This indicates that the node's implementation
     * is an alias for another node type's implementation
     * and is implemented in terms of that one.
     */
    ALIAS = 2
  };
}
