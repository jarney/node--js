#pragma once

#include <iterator>
#include <libxml/tree.h>

class XmlNodeWrapper {
public:

    struct Iterator {
	// Iterator tags here...
	using iterator_category = std::forward_iterator_tag;
	using difference_type   = std::ptrdiff_t;
	using value_type        = xmlNode;
	using pointer           = xmlNodePtr;  // or also value_type*
	using reference         = xmlNode&;  // or also value_type&
	
	// Iterator constructors here...
	Iterator(pointer node) : m_node(node) {}
	
	reference operator*() const { return *m_node; }
	pointer operator->() { return m_node; }
	XmlNodeWrapper get();// { return m_node; }
	
	// Prefix increment
	Iterator& operator++() {
	    m_node = xmlNextElementSibling(m_node);
	    return *this;
	}
	
	// Postfix increment
	Iterator operator++(int) {
	    Iterator tmp = *this;
	    ++(*this);
	    return tmp;
	}
	
	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_node == b.m_node; };
	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_node != b.m_node; };     
	
    private:
	pointer m_node;
    };

    XmlNodeWrapper(xmlNodePtr node);
    XmlNodeWrapper(xmlNodePtr node, bool is_owning);
    XmlNodeWrapper(std::string elementName);
    XmlNodeWrapper(const XmlNodeWrapper & other);
    ~XmlNodeWrapper();
    xmlNodePtr releasePointer();
    std::string getName(void) const;
    std::string getAttribute(const std::string & name) const;
    bool hasAttribute(const std::string & name) const;
    void setAttribute(const std::string & name, const std::string & value);

    /**
     * The child argument is not const
     * because the parent takes away
     * the child's pointer so that it
     * is no longer responsible for it
     * when it becomes our child.
     */
    void addChild(XmlNodeWrapper & child);
    
    Iterator begin();
    Iterator end();
private:
    xmlNodePtr _node;
    bool _is_owning;
};

