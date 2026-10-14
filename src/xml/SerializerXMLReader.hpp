#pragma once

#include "node--js/SerializerError.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace NodeJS {
    namespace xml {
      class SerializerXMLReader {
      public:
	  SerializerXMLReader(
	      std::string aFilename,
	      xmlDocPtr aDocument,
	      NodeJS::core::NodeModule & aNodeModule,
	      NodeJS::core::SerializerErrorReporter & aErrorReporter
	      );
	  ~SerializerXMLReader() = default;
	  bool read(void) const;
      private:
	  bool parseError(
	      NodeJS::core::SerializerErrorCode error_code,
	      const XmlNodeWrapper & element,
	      std::string message
	      ) const;
	  bool parseMetadata(
	      const XmlNodeWrapper & metadataContainer,
	      NodeJS::core::Metadata & metadata
	      ) const;
	  bool parseConnectionData(
	      XmlNodeWrapper parent,
	      NodeJS::core::ConnectionData & cd,
	      NodeJS::core::SerializerErrorCode missingCode,
	      NodeJS::core::SerializerErrorCode duplicateCode
	      ) const;
	  
	  bool parsePackage(void) const;
	  
	  bool parseDataType(XmlNodeWrapper node) const;
	  bool parseDataTypes(void) const;
	  
	  bool parsePort(
	      XmlNodeWrapper portNode,
	      NodeJS::core::NamedPorts & namedPorts,
	      std::string desc,
	      const NodeJS::core::NodeGraph *graph
	      ) const;
	  bool parsePorts(
	      XmlNodeWrapper nodeTypeNode,
	      NodeJS::core::NamedPorts & namedPorts,
	      std::string tag,
	      std::string desc,
	      const NodeJS::core::NodeGraph *graph
	      ) const;
	  bool parseNodeType(
	      XmlNodeWrapper nodeTypeNode
	      ) const;
	  bool parseNodeTypes(void) const;
	  
	  bool parseNode(
	      XmlNodeWrapper nodeNode,
	      NodeJS::core::NodeGraph & nodeGraph
	      ) const;
	  bool parseEdge(
	      XmlNodeWrapper edgeNode,
	      NodeJS::core::NodeGraph & node_graph
	      ) const;
	  bool parseScope() const;
	  bool parseGraph(
	      XmlNodeWrapper graphNode
	      ) const;
	  bool parseGraphs(void) const;
	  
	  std::string mFilename;
	  xmlDocPtr mDocument;
	  NodeJS::core::NodeModule & mNodeModule;
	  NodeJS::core::SerializerErrorReporter & mReporter;
	  XmlNodeWrapper mRoot;
	};
    }
}
