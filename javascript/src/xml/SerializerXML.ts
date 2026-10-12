import {NodeModule} from "../NodeModule.ts";
import {DataType} from "../DataType.ts";
import {NodeType} from "../NodeType.ts";
import {Node} from "../Node.ts";
import type {SerializerErrorReporter} from "../SerializerError.ts";
  
    // Data types need to be read before node types
    // even if they appear later in the file.

const ELEMENT_NODE: number = 1;

function getChildren(parent: ChildNode, tagName: string) : Element[] {
  var children: Element[] = [];
  for (var child of parent.childNodes) {
    if (child.nodeType == ELEMENT_NODE) {
      var childElement: Element = child as Element;
      if (childElement.tagName == tagName) {
	children.push(childElement);
      }
    }
  }
  return children;
}

function parseDataType(dataTypeNode: Element, nodeModule: NodeModule, reporter: SerializerErrorReporter) {
  var id: string | null = dataTypeNode.getAttribute("id");
  var name: string | null = dataTypeNode.getAttribute("name");
  if (id == null || name == null) {
    reporter.reportError(990, 23, "stream", "Missing id or name for data type");
    return false;
  }
    
  var dataType: DataType = new DataType(id!, name!);
  nodeModule.addDataType(dataType);

  return true;
}

export class SerializerXML {
  public read(
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : boolean {
    //console.log("Fetched HTTP data");
    //console.log(xmlString);
    const parser = new DOMParser();
    const doc = parser.parseFromString(xmlString, "application/xml");

    //console.log(xmlString);
    const root = doc.documentElement;
    for (var dataTypesNode of getChildren(root, "data-types")) {
      for (var dataTypeNode of getChildren(dataTypesNode, "data-type")) {
	if (!parseDataType(dataTypeNode, nodeModule, reporter)) {
	  return false;
	}
      }
    }
    console.log(nodeModule);
    return true;
  }
}
