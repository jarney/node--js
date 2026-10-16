#pragma once

#pragma once

#include "node--js/SerializerError.hpp"
#include "node--js/NodeModule.hpp"
#include "node--js/xml/XmlNodeWrapper.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace NodeJS {
    namespace xml {
      class SerializerXMLWriter {
      public:
	  SerializerXMLWriter(
	      std::string aFilename,
	      xmlDocPtr aDocument,
	      const NodeJS::core::NodeModule & aNodeModule,
	      NodeJS::core::SerializerErrorReporter & aErrorReporter
	      );
	  ~SerializerXMLWriter() = default;
	  bool write(void);
      private:

	  void writePackage(void);

	  void writeScopes(void);

	  void writeMetadata(
	      const NodeJS::core::Metadata & metadata,
	      XmlNodeWrapper & parentNode
	      );

	  void writeDataType(
	      const std::string & id,
	      const NodeJS::core::DataType & data_type,
	      XmlNodeWrapper dataTypesNode
	      );
	  
	  void writeDataTypes(void);
	  
	  void writeNamedPorts(
	      const NodeJS::core::NamedPorts & namedPorts,
	      XmlNodeWrapper portsNode
	      );
	  
	  void writeNodeType(
	      const std::string & id,
	      const NodeJS::core::NodeType & node_type,
	      XmlNodeWrapper nodeTypesNode
	      );
	  
	  void writeNodeTypes(void);
	  
	  void writeGraphNodeData(
	      const NodeJS::core::ConnectionData & data,
	      XmlNodeWrapper nodeNode
	      );

	  void writeGraphNode(
	      const NodeJS::core::Node & node,
	      XmlNodeWrapper nodeNode
	      );

	  void writeGraphNodes(
	      const NodeJS::core::NodeGraph & graph,
	      XmlNodeWrapper nodesNode
	      );
	  
	  void writeGraphEdges(
	      const NodeJS::core::NodeGraph & graph,
	      XmlNodeWrapper edgesNode
	      );
	  void writeGraph(
	      std::string id,
	      const NodeJS::core::NodeGraph & graph,
	      XmlNodeWrapper graphsNode
	      );
	  void writeGraphs(void);
	  
	  std::string mFilename;
	  xmlDocPtr mDocument;
	  const NodeJS::core::NodeModule & mNodeModule;
	  NodeJS::core::SerializerErrorReporter & mReporter;
	  XmlNodeWrapper mRoot;
	      
      };
    }
}
