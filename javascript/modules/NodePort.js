/**
 * A port is a part of a node that can exchange data.
 * Ports may play the role of either input or output data
 * in a node and may transmit data along the port.  Each port
 * has a data type and that data type must match between
 * the output of one node and the input of the next along
 * the connection.  A node may decide how it interprets
 * the data carried in the port, but ideally, multiple
 * nodes will agree on an encoding so that they can inter-operate.
 */


/**
 * Creates a port that can handle a particular
 * type of data.  Note that the port does not own
 * the data type, it must only obtain a reference
 * to a data type that has a longer lifetime than
 * any port.  Data types are assumed to be registered
 * in a registry and only destroyed after all of the
 * node types that refer to it have alredy been destroyed.
 *
 * Node ports are immutable once created.
 *
 * @param data_type Unique ID of the data type this
 *                  port supports.
 * @param description Human-readable description of
 *                    this port, for instance, the parameter name.
 * @param policy This indicates whether only one
 *               connection is permitted or whether
 *               multiple connections can be supported.
 */
function NodePort(aDataType, aDescription, aConnectionPolicy) {
    this.mDataType = aDataType;
    this.mDescription = aDescription;
    this.mConnectionPolicy = aConnectionPolicy;
};

/**
 * Connection policy for a port.  This determines
 * how many connections are permitted.  A port
 * may support only a single connection and may
 * support multiple connections.  It is up to
 * the node to decide how many connections are
 * reasonable and appropriate.  Note that when
 * data is carried along a connection, if there
 * are multiple connections, the order of processing
 * or arrival may not be predictable.
 */
NodePort.ConnectionPolicy = {
    /**
     * This value indicates that the port supports only
     * a single connection.
     */
    One: 0,
    /**
     * This value indicates that the port supports
     * multiple connections.
     */
    Multiple: 1
};

/**
 * Returns the type of data carried by this port.  Data types are
 * identified by this value and are completely abstract.  The underlying
 * implementation of each node type governs the specific interpretation
 * of the data being carried at this connection.  It is completely up to
 * node implementors to ensure that the data types are appropriately
 * namespaced to avoid ambiguities and to correctly handle data types.
 */
NodePort.prototype.getDataType = function() {
    return this.mDataType;
};

/**
 * Returns the description of this port's data.
 */
NodePort.prototype.getDescription = function() {
    return this.mDescription;
};
	    
/**
 * Returns the connection policy associated with this port.
 */
NodePort.prototype.getConnectionPolicy = function() {
    return this.mConnectionPolicy;
};

export {NodePort};
