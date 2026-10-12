import {NodeModule} from "../NodeModule.ts";
import {DataType} from "../DataType.ts";
import {NodeType} from "../NodeType.ts";
import {Node} from "../Node.ts";
import type {SerializerErrorReporter} from "../SerializerError.ts";
import {getElementLineno,
  ELEMENT_NODE,
  ATTRIBUTE_NODE,
  TEXT_NODE,
  CDATA_SECTION_NODE,
  PROCESSING_INSTRUCTION_NODE,
  COMMENT_NODE,
  DOCUMENT_NODE,
  DOCUMENT_TYPE_NODE,
  DOCUMENT_FRAGMENT_NODE
} from "./DomUtils.ts";

// Data types need to be read before node types
// even if they appear later in the file.

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

const SERIALIZER_ERROR_BAD_DATA_TYPE: number = 1000;
const SERIALIZER_ERROR_BAD_NODE_TYPE: number = 2000;

export class SerializerXML {
  mRoot: any;
  mFilename: string;
  
  constructor() {
    this.mRoot = undefined;
    this.mFilename = "unknown";
  }
  
  private parseDataType(dataTypeNode: Element, nodeModule: NodeModule, reporter: SerializerErrorReporter) {
    var id: string | null = dataTypeNode.getAttribute("id");
    var name: string | null = dataTypeNode.getAttribute("name");
    if (id == null || name == null) {
      var lineno: number = getElementLineno(this.mRoot, dataTypeNode);
      reporter.reportError(SERIALIZER_ERROR_BAD_DATA_TYPE, lineno, this.mFilename, "Missing id or name for data type");
      return false;
    }
    
    var dataType: DataType = new DataType(id!, name!);
    nodeModule.addDataType(dataType);
    
    return true;
  }

  public read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : boolean {
    //console.log("Fetched HTTP data");
    //console.log(xmlString);
    const parser = new DOMParser();
    const doc = parser.parseFromString(xmlString, "application/xml");
    this.mRoot = doc;
    this.mFilename = aFilename;

    //console.log(xmlString);
    const root = doc.documentElement;

    for (var packageNode of getChildren(root, "package")) {
      var lineno: number = getElementLineno(doc, packageNode);
      console.log("Package at line " + lineno);
    }
    
    for (var dataTypesNode of getChildren(root, "data-types")) {
      for (var dataTypeNode of getChildren(dataTypesNode, "data-type")) {
	if (!this.parseDataType(dataTypeNode, nodeModule, reporter)) {
	  return false;
	}
      }
    }
    console.log(doc);
    console.log(nodeModule);
    return true;
  }
}
