#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "node--js/ConnectionData.hpp"

namespace NodeJS {
    namespace core {

	class Metadata {
	public:
	    Metadata() = default;
	    ~Metadata() = default;

	    /**
	     * Metadata is data that can be associated
	     * with a node module that can be used by other
	     * extensions to the system.
	     */
	    const ConnectionData & getMetadata(std::string aMetadataNamespace) const;
	    ConnectionData & getMetadata(std::string aMetadataNamespace);
	    bool hasMetadata(std::string aMetadataNamespace) const;
	    std::vector<std::string> getNamespaces() const;
	private:
	    std::map<std::string, std::unique_ptr<ConnectionData>> mMetadata;
	};
    }
}
