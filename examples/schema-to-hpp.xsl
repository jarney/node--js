<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
	    xmlns:codegen="http://jarney.github.io/nodejs/comment">
  <xsl:output method="text"
	  indent="no"
	  encoding="string"
	  media-type="string"/>
<xsl:template match="/">

  <xsl:text disable-output-escaping="yes">
#pragma once

#include &lt;string&gt;
#include &lt;map&gt;
#include &lt;vector&gt;
#include &lt;memory&gt;

namespace NodeJS {
    namespace core {
</xsl:text>

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
      <xsl:text>    /**&#xa;</xsl:text>
      <xsl:text>     *&#xa;</xsl:text>
      <xsl:value-of select="codegen:comment"/>
      <xsl:text>     */&#xa;</xsl:text>

      <xsl:text>    std::string get_</xsl:text>
      <xsl:call-template name="get-name">
        <xsl:with-param name="element" select="."/>
      </xsl:call-template>
      <xsl:text>(void) const;&#xa;</xsl:text>
	
      <xsl:text>    void set_</xsl:text>
      <xsl:call-template name="get-name">
        <xsl:with-param name="element" select="."/>
      </xsl:call-template>
      <xsl:text>(std::string value);&#xa;</xsl:text>
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
      <xsl:text>    std::string </xsl:text>
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

</xsl:stylesheet>
