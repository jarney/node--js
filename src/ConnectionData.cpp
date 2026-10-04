#include "node--js/ConnectionData.hpp"

using namespace NodeJS::core;

ConnectionData::ConnectionData()
{}

ConnectionData::ConnectionData(const ConnectionData & other)
    : mData(other.mData)
{}

void
ConnectionData::setValue(std::string key, std::string value)
{
    if (!hasValue(key)) {
	mData[key] = value;
    }
    else {
	mData[key] = getValue(key) + value;
    }
}

std::string
ConnectionData::getValue(std::string key, std::string default_value) const
{
    const auto it = mData.find(key);
    if (it == mData.end()) {
	return default_value;
    }
    return it->second;
}
std::string
ConnectionData::getValue(std::string key) const
{
    return getValue(key, "");
}

bool
ConnectionData::hasValue(std::string key) const
{
    const auto it = mData.find(key);
    if (it == mData.end()) {
	return false;
    }
    return true;
}

const std::map<std::string, std::string> &
ConnectionData::getData() const
{
    return mData;
}

