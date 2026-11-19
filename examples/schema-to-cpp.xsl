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
    
    
    <xsl:for-each select="xsd:schema/xsd:complexType">
      <xsl:variable name="className">
	<xsl:call-template name="get-name">
	  <xsl:with-param name="element" select="."/>
	</xsl:call-template>
      </xsl:variable>
	
      <xsl:text>#include &lt;</xsl:text>
      <xsl:value-of select="$className"/>
      <xsl:text>.hpp&gt;&#xa;</xsl:text>
      
      <xsl:text>using namespace NodeJS::core;&#xa;</xsl:text>

      <xsl:value-of select="$className"/>
      <xsl:text>::</xsl:text>
      <xsl:value-of select="$className"/>
      <xsl:text>()&#xa;</xsl:text>
      <xsl:text>{}&#xa;</xsl:text>

      <!-- Member functgions -->
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

	<!-- Const accessor -->
	<xsl:text>const </xsl:text>
	<xsl:value-of select="$memberType"/>
	<xsl:text> &amp;</xsl:text>
	<xsl:value-of select="$className"/>
	<xsl:text>::get_</xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text>(void) const&#xa;</xsl:text>
	<xsl:text>{&#xa;</xsl:text>
	<xsl:text>    return </xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text>;&#xa;</xsl:text>
	<xsl:text>}&#xa;</xsl:text>

	<xsl:text>&#xa;</xsl:text>

	<!-- Mutable accessor -->
	<xsl:value-of select="$memberType"/>
	<xsl:text> &amp;</xsl:text>
	<xsl:value-of select="$className"/>
	<xsl:text>::get_</xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text>(void)&#xa;</xsl:text>
	<xsl:text>{&#xa;</xsl:text>
	<xsl:text>    return </xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text>;&#xa;</xsl:text>
	<xsl:text>}&#xa;</xsl:text>
	
	<xsl:text>&#xa;</xsl:text>

	<!-- Mutable mutator -->
	<xsl:text>void </xsl:text>
	<xsl:value-of select="$className"/>
	<xsl:text>::set_</xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text>(</xsl:text>
	<xsl:value-of select="$memberType"/>
	<xsl:text> _value)&#xa;</xsl:text>
	<xsl:text>{&#xa;</xsl:text>
	<xsl:value-of select="$memberName"/>
	<xsl:text> = _value;&#xa;</xsl:text>
	<xsl:text>}&#xa;</xsl:text>
	
	<xsl:text>&#xa;</xsl:text>
	
      </xsl:for-each>
      
    </xsl:for-each>
    
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
