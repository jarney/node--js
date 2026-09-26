#include <node--js/DataType.hpp>

using namespace NodeJS::core;

DataType::DataType(
    std::string id,
    std::string name
    )
    : mId(id)
    , mName(name)
{}

const std::string &
DataType::getId() const
{ return mId; }

const std::string &
DataType::getName() const
{ return mName; }

bool
DataType::operator==(const DataType & other) const
{
    return other.mId == mId;
}

