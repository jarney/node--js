<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
	    xmlns:codegen="http://jarney.github.io/nodejs/comment">
  <xsl:output method="xml"
	  indent="yes"
	  encoding="UTF-8"
	  media-type="string"
	  />

  <xsl:template match="/">
    <xsl:copy>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="@*|xsd:*">
    <xsl:copy>
      <xsl:apply-templates select="@*|xsd:*"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
