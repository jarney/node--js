#pragma once

#include <string>
#include <map>

namespace NodeJS {
    namespace core {
	/**
	 * A DataType in NodeJS represents the type of data
	 * flowing along a connection between nodes.  The physical
	 * storage of data types is abstract and only identified by a
	 * name.  The implementation of the physical means of connection
	 * is governed by the specific nodes and the implementing platform.
	 */
	class DataType {
	public:
	    DataType(std::string tag);
	    DataType(const DataType & other) = default;
	    DataType & operator=(const DataType & other) = default;
	    ~DataType() = default;
	    const std::string & getName() const;

	    bool operator==(const DataType & other) const;
	private:
	    std::string mName;
	};
    }
}
