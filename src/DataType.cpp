#include <node--js/DataType.hpp>

using namespace NodeJS::core;

DataType::DataType(
    std::string id,
    std::string package,
    std::string name
    )
    : mId(id)
    , mFullyQualifiedId(package + std::string(".") + id)
    , mName(name)
{}

const std::string &
DataType::getLocalId() const
{ return mId; }

const std::string &
DataType::getFullyQualifiedId() const
{ return mFullyQualifiedId; }

const std::string &
DataType::getName() const
{ return mName; }

bool
DataType::operator==(const DataType & other) const
{
    return other.mFullyQualifiedId == mFullyQualifiedId;
}

bool
DataType::operator!=(const DataType & other) const
{
    return other.mFullyQualifiedId != mFullyQualifiedId;
}

