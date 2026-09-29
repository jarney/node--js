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
	    /**
	     * Creates a new data type.
	     *
	     * @param id Unique identifier
	     * @param name Name which is human-readable.
	     */
	    DataType(std::string id, std::string name);
	    
	    /**
	     * This makes a copy of a data type.  This is
	     * needed in order to hold the DataType objects
	     * in an STL container.
	     */
	    DataType(const DataType & other) = default;

	    /**
	     * It is possible to copy one data type into another.
	     * This is important in order to allow DataType objects
	     * to be held in STL containers.
	     */
	    DataType & operator=(const DataType & other) = default;

	    /**
	     * Destructor, nothing to see here.
	     */
	    ~DataType() = default;

	    /**
	     * Returns the unique identifier for this data type.
	     */
	    const std::string & getId() const;

	    /**
	     * Returns the human-readable name for this data type.
	     */
	    const std::string & getName() const;

	    /**
	     * Two data types compare as equal if and only if
	     * their unique IDs are the same.  Note that their
	     * names are permitted to differ and still be
	     * considered the same.
	     */
	    bool operator==(const DataType & other) const;
	private:
	    std::string mId;
	    std::string mName;
	};
    }
}
