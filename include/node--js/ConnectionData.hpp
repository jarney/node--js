#pragma once

#include <string>
#include <map>

namespace NodeJS {
    namespace core {

	/**
	 * Connection data represents the data that
	 * flows along edges in a NodeJS graph.
	 *
	 * Internally, this is represented as a collection of name/value
	 * pairs where each input or output may place data into the map
	 * for arrival at the other end of a connection.  This makes it
	 * a very general-purpose way to move data between nodes
	 * because the nodes may be free to encode the strings in any
	 * arbitrary way.
	 */
	class ConnectionData {
	public:
	    ConnectionData();
	    ConnectionData(const ConnectionData & other);
	    ConnectionData & operator=(const ConnectionData & ) = default;
	    ~ConnectionData() = default;

	    /**
	     * Places data at key 'key' into the connection.
	     */
	    void setValue(std::string key, std::string value);

	    /**
	     * Retrieves the value stored at 'key' from the connection,
	     * or 'default_value' if no value was stored.
	     */
	    std::string getValue(std::string key, std::string default_value) const;

	    /**
	     * Returns the value at key 'key' from the
	     * connection.  If no value exists at that
	     * key, this is undefined behavior.  Therefore,
	     * you must call 'hasValue' first to verify that
	     * there is data stored at that key first.
	     */
	    std::string getValue(std::string key) const;

	    /**
	     * Returns true if there is a value stored at this key.
	     */
	    bool hasValue(std::string key) const;

	    const std::map<std::string, std::string> & getData() const;
	    
	private:
	    std::map<std::string, std::string> mData;
	};

    } // End core
} // End JNodes
