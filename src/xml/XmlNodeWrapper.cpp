#include "node--js/xml/XmlNodeWrapper.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

XmlNodeWrapper
XmlNodeWrapper::Iterator::get() {
    return XmlNodeWrapper(m_node);
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

XmlNodeWrapper::XmlNodeWrapper(xmlNodePtr node)
    : _node(node)
{}

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
XmlNodeWrapper::addChild(const XmlNodeWrapper & other)
{
    xmlAddChild(_node, other._node);
}


