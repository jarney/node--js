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
			XmlNodeWrapper & element,
			std::string message
		       ) const;
	bool parseMetadata(
			  XmlNodeWrapper & metadataContainer,
			  NodeJS::core::Metadata & metadata
			 ) const;
	bool readDataType(
			  XmlNodeWrapper node
			 ) const;
	
	bool readDataTypes(
			   XmlNodeWrapper dataTypesNode
			  ) const;
	bool readPackage(
			 XmlNodeWrapper packageNode
			) const;
	bool readNamedPorts(
			    NodeJS::core::NamedPorts & namedPorts,
			    XmlNodeWrapper inputsNode
			   ) const;
	bool readNodeType(
			  XmlNodeWrapper nodeTypeNode
			 ) const;
	bool readNodeTypes(
			   XmlNodeWrapper nodeTypesNode
			  ) const;
	bool readGraphNodeData(
			       NodeJS::core::ConnectionData & node_data,
			       XmlNodeWrapper nodeNode
			      ) const;
	bool readGraphNode(
			     NodeJS::core::NodeGraph & node_graph,
			     XmlNodeWrapper nodeNode
			    ) const;
	  bool readGraphNodes(
			      NodeJS::core::NodeGraph & node_graph,
			      XmlNodeWrapper nodesNode
			     ) const;
	  bool readGraphEdge(
			     NodeJS::core::NodeGraph & node_graph,
			     XmlNodeWrapper edgeNode
			    ) const;
	  bool readGraphEdges(
			      NodeJS::core::NodeGraph & node_graph,
			      XmlNodeWrapper nodesNode
			     ) const;
	  bool readGraphScope(
			      NodeJS::core::NodeGraph & node_graph,
			      XmlNodeWrapper scopeNode
			     ) const;
	  bool readGraph(
			 XmlNodeWrapper graphNode
			) const;
	  bool readGraphs(
			  XmlNodeWrapper nodeTypesNode
			 ) const;
	  std::string mFilename;
	  xmlDocPtr mDocument;
	  NodeJS::core::NodeModule & mNodeModule;
	  NodeJS::core::SerializerErrorReporter & mReporter;
	};
    }
}
