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
    <xsl:text>#include &lt;string&gt;</xsl:text>
  </xsl:template>
</xsl:stylesheet>
