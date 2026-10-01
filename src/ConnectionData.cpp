#include "node--js/ConnectionData.hpp"

using namespace NodeJS::core;

ConnectionData::ConnectionData()
{}

ConnectionData::ConnectionData(const ConnectionData & other)
    : _data(other._data)
{}

void
ConnectionData::setValue(std::string key, std::string value)
{
    _data[key] = value;
}

std::string
ConnectionData::getValue(std::string key, std::string default_value) const
{
    const auto it = _data.find(key);
    if (it == _data.end()) {
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
    const auto it = _data.find(key);
    if (it == _data.end()) {
	return false;
    }
    return true;
}

const std::map<std::string, std::string> &
ConnectionData::getData() const
{
    return _data;
}

