#include <catch2/catch_all.hpp>

#include "node--js/xml/XmlNodeWrapper.hpp"
#include <libxml/tree.h>

using namespace NodeJS::xml;

TEST_CASE("xml::XmlNodeWrapper ownership text", "[NodeJS][xml][XmlNodeWrapper]")
{
    // This pointer is owned by us and we're not transferring it anywhere.
    XmlNodeWrapper otherNode("q");
}

TEST_CASE("xml::XmlNodeWrapper pointer logic test", "[NodeJS][xml][XmlNodeWrapper]")
{
    // This pointer is owned by someone else,
    // and we didn't do anything with it,
    // so we can't free it.

    // This is an explicit test because
    xmlNodePtr x = xmlNewNode(nullptr, BAD_CAST "test-element");
    XmlNodeWrapper newnode(x, false);
    xmlFreeNode(x);

}

TEST_CASE("xml::XmlNodeWrapper text content tests", "[NodeJS][xml][XmlNodeWrapper]")
{
    // If we don't add content, it's the empty string.
    xmlNodePtr nocontent = xmlNewNode(nullptr, BAD_CAST "no-text-content");
    XmlNodeWrapper nocontentwrap(nocontent, true);
    CHECK(nocontentwrap.getContent() == "");

    // If there's no node, the content is empty.
    XmlNodeWrapper nonodewrap(nullptr, false);
    CHECK(nonodewrap.getContent() == "");

    // If we deliberately add null, the content is the empty string.
    xmlNodePtr nulltext = xmlNewNode(nullptr, BAD_CAST "nulltext");
    xmlNodePtr textContentNode = xmlNewText(nullptr);
    xmlAddChild(nulltext, textContentNode);
    XmlNodeWrapper nulltextwrap(nulltext, true);
    CHECK(nulltextwrap.getContent() == "");

    // If we add a non-content chidl, the content is the empty string.
    XmlNodeWrapper nonContentChild("noncontent");
    XmlNodeWrapper element("element");
    nonContentChild.addChild(element);
    CHECK(nonContentChild.getContent() == "");
    
}

