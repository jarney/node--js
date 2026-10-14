#pragma once

#include <string>

namespace NodeJS {
    namespace core {

	/**
	 * A qualified identifier is an identifier used for things
	 * like node types, data types, graphs, etc where
	 * the first portion of the name is the 'package name' or
	 * 'namespace' and the second portion is the name of the item
	 * within that namespace.  This is useful for looking up
	 * objects across modules and stitching the modules into
	 * a whole program.
	 */
	class QualifiedIdentifier {
	public:
	    QualifiedIdentifier(std::string aQualifiedIdentifier);
	    ~QualifiedIdentifier() = default;
	    /**
	     * Return the last portion of the name
	     * after the last '.'.
	     */
	    const std::string & getName() const;
	    /**
	     * Returns true if there was a dot and anything
	     * before it.
	     */
	    bool isQualified() const;
	    /**
	     * Returns the part of the name before the dot.
	     */
	    const std::string & getNamespace() const;
	private:
	    std::string mName;
	    bool mIsQualified;
	    std::string mNamespace;
	};
	
    }
}
