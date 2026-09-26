#include <node--js/DataType.hpp>

using namespace NodeJS::core;

DataType::DataType(std::string tag)
    : mName(tag)
{}

const std::string &
DataType::getName() const
{ return mName; }

bool
DataType::operator==(const DataType & other) const
{
    return other.mName == mName;
}

