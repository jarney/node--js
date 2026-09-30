#include "node--js/xml/XmlNodeWrapper.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

XmlNodeWrapper
XmlNodeWrapper::Iterator::get() {
    return XmlNodeWrapper(m_node, false);
}

XmlNodeWrapper::Iterator
XmlNodeWrapper::begin()
{
    return Iterator(xmlFirstElementChild(_node));
}
XmlNodeWrapper::Iterator
XmlNodeWrapper::end()
{
    return Iterator(nullptr);
}

XmlNodeWrapper::XmlNodeWrapper(xmlNodePtr node, bool is_owning)
    : _node(node)
    , _is_owning(is_owning)
{}

XmlNodeWrapper::XmlNodeWrapper(std::string elementName)
    : _node(xmlNewNode(nullptr, BAD_CAST elementName.c_str()))
    , _is_owning(true)
{}

xmlNodePtr
XmlNodeWrapper::releasePointer()
{
    xmlNodePtr toRelease = _node;
    _node = nullptr;
    return toRelease;
}

XmlNodeWrapper::XmlNodeWrapper(const XmlNodeWrapper & source)
    : _node(source._node)
    , _is_owning(false)
{}

XmlNodeWrapper::~XmlNodeWrapper()
{
    if (_node != nullptr && _is_owning) {
	// If we still have this
	// pointer when we leave scope,
	// we are responsible for it.
	// We own it until it has been passed off
	// to another node.
	xmlFree((void*)_node);
	_node = nullptr;
    }
}

std::string
XmlNodeWrapper::getName(void) const
{
    return std::string((const char *)_node->name);
}

std::string
XmlNodeWrapper::getAttribute(const std::string & name) const
{
    const xmlChar *id = xmlGetProp(_node, BAD_CAST name.c_str());
    std::string str((const char *)id);
    xmlFree((void*)id);
    return str;
}
bool
XmlNodeWrapper::hasAttribute(const std::string & name) const
{
    const xmlAttr *attr = xmlHasProp(_node, BAD_CAST name.c_str());
    return attr != nullptr;
}


void
XmlNodeWrapper::setAttribute(const std::string & name, const std::string & value)
{
    xmlSetProp(_node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
}

void
XmlNodeWrapper::addChild(XmlNodeWrapper & other)
{
    xmlAddChild(_node, other._node);
    other._node = nullptr;
}


