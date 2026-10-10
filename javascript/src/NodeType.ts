/**
 * This enum represents the visibility of a node type.
 * This determines whether graphs outside this package are
 * permitted to access it.
 */
enum Visibility {
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
enum Type {
    /**
     * This indicates that the node's implementation is
     * provided by the underlying runtime.
     */
    NATIVE = 0,
    /**
     * This indicates that the node's implementation is
     * provided by a node graph consisting of other node types.
     */
    GRAPH = 1
};



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

class NodeType {
    mId: string;
    mVisibility: Visibility;
    mType: Type;
    mInputsByName: { [key: string] : string };
    mOutputsByName: { [key: string] : string };

    constructor() {
        this.mId = "";
        this.mVisibility = Visibility.PRIVATE;
        this.mType = Type.NATIVE;
        this.mInputsByName = {};
        this.mOutputsByName = {};
    }

    public getId(): string {
        return this.mId;
    }

    public setId(aId: string) {
        this.mId = aId;
    }

    public setVisibility(aVisibility: Visibility) {
        this.mVisibility = aVisibility;
    }
    public setType(aType: Type) {
        this.mType = aType;
    }
    public getVisibility() : Visibility {
        return this.mVisibility;
    }
    public getType() : Type {
        return this.mType;
    }
}

export {NodeType};
