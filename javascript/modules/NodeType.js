

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


function NodeType() {
    this.mVisibility = NodeType.Visibility.PRIVATE;
    this.mType = NodeType.Visibility.NATIVE;

    this.mInputsByName = {};
    this.mOutputsByName = {};
    this.mInputs = [];
    this.mOutputs = [];
    this.mInputNames = [];
    this.mOutputNames = [];
}

/**
 * This enum represents the visibility of a node type.
 * This determines whether graphs outside this package are
 * permitted to access it.
 */
NodeType.Visibility = {
    /**
     * This indicates that a node type is public and may
     * be used in any graph context.
     */
    PUBLIC: 0,
    /**
     * This indicates that a node type is private
     * and may be used only in the context of the node module
     * where it was declared.
     */
    PRIVATE: 1
};

/**
 * This enum represents the declaration of how a node
 * is implemented.
 */
NodeType.Type = {
    /**
     * This indicates that the node's implementation is
     * provided by the underlying runtime.
     */
    NATIVE: 0,
    /**
     * This indicates that the node's implementation is
     * provided by a node graph consisting of other node types.
     */
    GRAPH: 1
};


NodeType.prototype.getId = function() {
    return this.mId;
};

NodeType.prototype.setId = function(aId) {
    this.mId = aId;
};
	    
/**
 * Returns the visibility of this node type.
 * This determines whether the node type is
 * visible outside the scope of this module
 * or not.  This is useful for organizing
 * code into modules where part of the
 * impelementation is hidden.
 */
NodeType.prototype.getVisibility = function() {
    return this.mVisibility;
};

NodeType.prototype.setVisibility = function(aVisibility) {
    this.mVisibility = aVisibility;
};

/**
 * This returns the implementation type
 * for this type.  Some types may be
 * implemented on the underlying runtime
 * rather than being implemented in terms
 * of other nodes as a graph.
 */
NodeType.prototype.getType = function() {
    return this.mType;
};

/**
 * This sets the implementation as either graph
 * or native.
 */
NodeType.prototype.setType = function(aType) {
    this.mType = aType;
};


/**
 * Adds a new port to the node type.
 * This returns false if the node already existed.
 */
NodeType.prototype.addInputPort = function(aName, aPort) {
    if (this.hasInputPort(aName)) return false;
    this.mInputsByName[aName] = aPort;
    this.mInputs.push(aPort);
    this.mInputNames.push(aName);
    return true;
};

NodeType.prototype.getInputPortByName = function(aName) {
    return this.mInputsByName[aName];
};

NodeType.prototype.getInputPortByIndex = function(aIndex) {
    return this.mInputs[aIndex];
};

NodeType.prototype.hasInputPort = function(aName) {
    return aName in this.mInputsByName;
};

NodeType.prototype.getInputPortName = function(aIndex) {
    return this.mInputNames[aIndex];
};

NodeType.prototype.getInputPortCount = function() {
    return this.mInputs.length;
};

NodeType.prototype.addOutputPort = function(aName, aPort) {
    if (this.hasOutputPort(aName)) return false;
    this.mOutputsByName[aName] = aPort;
    this.mOutputs.push(aPort);
    this.mOutputNames.push(aName);
    return true;
};

NodeType.prototype.getOutputPortByName = function(aName) {
    return this.mOutputsByName[aName];
};

NodeType.prototype.getOutputPortByIndex = function(aIndex) {
    return this.mOutputs[aIndex];
};

NodeType.prototype.hasOutputPort = function(aName) {
    return aName in this.mOutputsByName;
};

NodeType.prototype.getOutputPortName = function(aIndex) {
    return this.mOutputNames[aIndex];
};

NodeType.prototype.getOutputPortCount = function() {
    return this.mOutputs.length;
};

export {NodeType};
