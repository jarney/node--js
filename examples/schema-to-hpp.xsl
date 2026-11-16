<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
	    xmlns:codegen="http://jarney.github.io/nodejs/comment">
  <xsl:output method="text"
	  indent="no"
	  encoding="string"
	  media-type="string"/>
<!--    <xsl:for-each select="xsd:schema/xsd:complexType">
     </xsl:for-each>
-->
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
/**
 * <xsl:value-of select="codegen:comment"/>
 */
class <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template> {
public:
    <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>();
    ~<xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>() = default;

    <xsl:for-each select="xsd:attribute">
    /**
     * <xsl:value-of select="codegen:comment"/>
     */
    std::string get_<xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>(void) const;
    void set_<xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>(std::string value);
    </xsl:for-each>

private:

    <xsl:choose>
      <xsl:when test="count(xsd:sequence) = 1">
	std::list&lt;<xsl:call-template name="strip-node-prefix"><xsl:with-param name="value"><xsl:value-of select="xsd:sequence/xsd:element/@type"/></xsl:with-param></xsl:call-template>&gt; <xsl:call-template name="get-name"><xsl:with-param name="element" select="xsd:sequence/xsd:element"/></xsl:call-template>;
      </xsl:when>
      <xsl:otherwise>
    <xsl:for-each select="xsd:attribute">
    std::string <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>;</xsl:for-each>
    <xsl:for-each select="xsd:all/xsd:element">
      <xsl:call-template name="strip-node-prefix"><xsl:with-param name="value"><xsl:value-of select="@type"/></xsl:with-param></xsl:call-template><xsl:text> </xsl:text> <xsl:call-template name="get-name"><xsl:with-param name="element" select="."/></xsl:call-template>;
    </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
    
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
