<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
	    xmlns:codegen="http://jarney.github.io/nodejs/comment">
  <xsl:output method="text"
	  indent="no"
	  encoding="UTF-8"
	  media-type="string"/>
<xsl:template match="/">

  <xsl:text>
#pragma once

#include &lt;string&gt;
#include &lt;map&gt;
#include &lt;vector&gt;
#include &lt;memory&gt;

</xsl:text>

<xsl:for-each select="xsd:schema/codegen:include">
  <xsl:text>#include "</xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text>.hpp"&#xa;</xsl:text>
</xsl:for-each>

<xsl:text>
namespace NodeJS {
    namespace core {
</xsl:text>

  <!-- Enumerations -->
  <xsl:for-each select="xsd:schema/xsd:simpleType">
    <xsl:if test="count(xsd:restriction/xsd:enumeration) &gt; 0">
      <xsl:text>typedef enum {&#xa;</xsl:text>
      <xsl:for-each select="xsd:restriction/xsd:enumeration">
	<xsl:text>    </xsl:text>
	<xsl:value-of select="@codegen:name"/>
	<xsl:if test="position() != last()">
	  <xsl:text>,</xsl:text>
	</xsl:if>
	<xsl:text>&#xa;</xsl:text>
      </xsl:for-each>
      <xsl:text>} </xsl:text>
      <xsl:call-template name="get-name">
	<xsl:with-param name="element" select="."/>
      </xsl:call-template>
      <xsl:text>;&#xa;</xsl:text>
    </xsl:if>
  </xsl:for-each>

  <!-- Composite Types -->
  <xsl:for-each select="xsd:schema/xsd:complexType">
    <xsl:text>/**&#xa;</xsl:text>
    <xsl:text> *&#xa;</xsl:text>
    <xsl:value-of select="codegen:comment"/>
    <xsl:text> */&#xa;</xsl:text>
      
    <xsl:text>class </xsl:text>
    <xsl:call-template name="get-name">
      <xsl:with-param name="element" select="."/>
    </xsl:call-template>
    <xsl:text>{&#xa;</xsl:text>

    <xsl:text>public:&#xa;</xsl:text>
      
    <!-- Default Constructor -->
    <xsl:text>    </xsl:text>
    <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>
    <xsl:text>();&#xa;</xsl:text>

    <!-- Copy constructor (automatic) -->
    <xsl:text>    </xsl:text>
    <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>
    <xsl:text>(const </xsl:text>
    <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>
    <xsl:text> &amp; other) = default;&#xa;</xsl:text>

    <!-- Default Destructor (automatic) -->
    <xsl:text>    ~</xsl:text>
    <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>
    <xsl:text>() = default;&#xa;</xsl:text>

    <xsl:for-each select="xsd:attribute">
      <xsl:variable name="memberType">
	<xsl:call-template name="get-type">
	  <xsl:with-param name="type" select="@type"/>
	</xsl:call-template>
      </xsl:variable>
      <xsl:variable name="memberName">
	<xsl:call-template name="get-name">
          <xsl:with-param name="element" select="."/>
	</xsl:call-template>
      </xsl:variable>

      <xsl:text>    /**&#xa;</xsl:text>
      <xsl:text>     *&#xa;</xsl:text>
      <xsl:value-of select="codegen:comment"/>
      <xsl:text>     */&#xa;</xsl:text>

      <xsl:text>    const </xsl:text>
      <xsl:value-of select="$memberType"/>
      <xsl:text> &amp;get_</xsl:text>
      <xsl:value-of select="$memberName"/>
      <xsl:text>(void) const;&#xa;</xsl:text>

      <xsl:text>    </xsl:text>
      <xsl:value-of select="$memberType"/>
      <xsl:text> &amp;get_</xsl:text>
      <xsl:value-of select="$memberName"/>
      <xsl:text>(void);&#xa;</xsl:text>
      
      <xsl:text>    void set_</xsl:text>
      <xsl:value-of select="$memberName"/>
      <xsl:text>(</xsl:text>
      <xsl:value-of select="$memberType"/>
      <xsl:text> value);&#xa;</xsl:text>
    </xsl:for-each>

    <xsl:text>private:&#xa;</xsl:text>

<!-- This is the private data declared for this type. -->
    <xsl:choose>
      <xsl:when test="count(xsd:sequence) = 1">
	<xsl:choose>
	  <xsl:when test="string-length(xsd:sequence/xsd:element/@codegen:map-by) &gt; 0">
	    <xsl:text>    std::map&lt;std::string, </xsl:text>
	    <xsl:call-template name="strip-node-prefix">
	      <xsl:with-param name="value">
		<xsl:value-of select="xsd:sequence/xsd:element/@type"/>
	      </xsl:with-param>
	    </xsl:call-template>
	    <xsl:text>&gt; </xsl:text>
	    <xsl:call-template name="get-name">
	      <xsl:with-param name="element" select="xsd:sequence/xsd:element"/>
	    </xsl:call-template>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:text>    std::vector&lt;</xsl:text>
	    <xsl:call-template name="strip-node-prefix">
	      <xsl:with-param name="value">
		<xsl:value-of select="xsd:sequence/xsd:element/@type"/>
	      </xsl:with-param>
	    </xsl:call-template>
	    <xsl:text>&gt; </xsl:text>
	    <xsl:call-template name="get-name">
	      <xsl:with-param name="element" select="xsd:sequence/xsd:element"/>
	    </xsl:call-template>
	  </xsl:otherwise>
	</xsl:choose>
	<xsl:text>;&#x0a;</xsl:text>
      </xsl:when>
    </xsl:choose>

    <xsl:for-each select="xsd:attribute">
      <xsl:text>    </xsl:text>
      <xsl:call-template name="get-type">
	<xsl:with-param name="type" select="@type"/>
      </xsl:call-template>
      <xsl:text> </xsl:text>
      <xsl:call-template name="get-name">
        <xsl:with-param name="element" select="."/>
      </xsl:call-template>
      <xsl:text>;&#xa;</xsl:text>
    </xsl:for-each>
    
    <xsl:for-each select="xsd:all/xsd:element">
      <xsl:text>    </xsl:text>
      <xsl:call-template name="strip-node-prefix">
	<xsl:with-param name="value">
	  <xsl:value-of select="@type"/>
	</xsl:with-param>
      </xsl:call-template>
      <xsl:text> </xsl:text>
      <xsl:call-template name="get-name">
	<xsl:with-param name="element" select="."/>
      </xsl:call-template>
      <xsl:text>;&#xa;</xsl:text>
    </xsl:for-each>
    
};
</xsl:for-each>
<xsl:text>
    }
}
</xsl:text>

</xsl:template>

  <xsl:template name="strip-node-prefix">
    <xsl:param name="value"/>
    <xsl:value-of select="substring($value, 6)"/>
  </xsl:template>
  
  <xsl:template name="get-name">
    <xsl:param name="element"/>
    <xsl:choose>
      <xsl:when test="string-length($element/@codegen:name) &gt; 0">
        <xsl:value-of select="$element/@codegen:name"/>
      </xsl:when>
      <xsl:otherwise>
	<xsl:value-of select="$element/@name"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="get-type">
    <xsl:param name="type"/>
    <xsl:choose>
      <xsl:when test="string-length($type) &gt; 0">
	<xsl:call-template name="strip-node-prefix">
	  <xsl:with-param name="value" select="$type"/>
	</xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
	<xsl:text>std::string</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
