#include "node--js/Metadata.hpp"

using namespace NodeJS::core;

const ConnectionData &
Metadata::getMetadata(std::string aMetadataNamespace) const
{
    static ConnectionData mEmptyMetadata;

    // Semantics here are lazy initialization.  If the
    // namespace doesn't exist, go ahead and create an empty one.
    const auto & metadataIt = mMetadata.find(aMetadataNamespace);
    if (metadataIt != mMetadata.end()) {
	ConnectionData & metadata = *metadataIt->second.get();
	return metadata;
    }

    // Return an empty metadata for our caller
    // to use.
    return mEmptyMetadata;
}

ConnectionData &
Metadata::getMetadata(std::string aMetadataNamespace)
{
    // Semantics here are lazy initialization.  If the
    // namespace doesn't exist, go ahead and create an empty one.
    const auto & metadataIt = mMetadata.find(aMetadataNamespace);
    if (metadataIt != mMetadata.end()) {
	ConnectionData & metadata = *metadataIt->second.get();
	return metadata;
    }

    // Create a new empty metadata and insert it
    // so our caller can use it and populate as needed.
    std::unique_ptr<ConnectionData> metadataPtr = std::make_unique<ConnectionData>();
    ConnectionData & metadata = *metadataPtr.get();
    mMetadata.insert(std::pair(aMetadataNamespace, std::move(metadataPtr)));
    return metadata;
}

bool
Metadata::hasMetadata(std::string aMetadataNamespace) const
{
    const auto & metadataIt = mMetadata.find(aMetadataNamespace);
    if (metadataIt != mMetadata.end()) {
	return true;
    }
    return false;
}

std::vector<std::string>
Metadata::getNamespaces() const
{
    std::vector<std::string> namespaces;

    for (const auto & metadataIt : mMetadata) {
	namespaces.push_back(metadataIt.first);
    }
    
    return namespaces;
}
