#pragma once

#include "node--js/DataType.hpp"

#include <string>

namespace NodeJS {
    namespace core {

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
	class NodePort {
	public:
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
	    typedef enum {
		One,
		Multiple
	    } ConnectionPolicy;

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
	     */
	    NodePort(std::string data_type, std::string description, ConnectionPolicy policy);

	    /**
	     * Creates a port with the most common connection
	     * policy of 'One'.
	     */
	    NodePort(std::string data_type, std::string description);

	    /**
	     * Destroys a port.  Note that this destructor does not
	     * have to be virtual because this is not sub-classed anywhere.
	     */
	    ~NodePort() = default;

	    /**
	     * Returns the type of data carried by this port.
	     */
	    const std::string & getDataType() const;

	    const std::string & getDescription() const;
	    
	    /**
	     * Returns the connection policy associated with this port.
	     */
	    const ConnectionPolicy & getConnectionPolicy() const;
	    
	private:
	    std::string mDataType;
	    std::string mDescription;
	    ConnectionPolicy mConnectionPolicy;
	};
    } // End core
} // End JNodes
