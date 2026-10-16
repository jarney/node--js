import type {SerializerErrorReporter} from "../SerializerError";
import {NodeModule} from "../NodeModule";
import {ModuleLoader} from "../ModuleLoader";
import {DataType} from "../DataType";
import {NodeType} from "../NodeType";
import {Metadata} from "../Metadata";
import {ConnectionData} from "../ConnectionData";
import type {NodeId} from "../Definitions";
import type {Serializer} from "../Serializer";
import {Node} from "../Node";
import {Edge} from "../Edge";
import {NodePort} from "../NodePort";
import {NodeGraph} from "../NodeGraph";
import {NamedPorts} from "../NamedPorts";
import {QualifiedIdentifier} from "../QualifiedIdentifier";
import {SerializerErrorCode, SerializerErrorReporterNil} from "../SerializerError";
import {DomUtils} from "./DomUtils";
import {SerializerXML} from "./SerializerXML";

export class SerializerXMLWriter {
  mFilename: string;
  mDocument: Document;
  mNodeModule: NodeModule;
  mReporter: SerializerErrorReporter;
  mRoot: Element;

  constructor(
    aFilename: string,
    aDocument: Document,
    aNodeModule: NodeModule,
    aReporter: SerializerErrorReporter
  ) {
    this.mFilename = aFilename;
    this.mDocument = aDocument;
    this.mNodeModule = aNodeModule;
    this.mReporter = aReporter;
    
    this.mRoot = this.mDocument.createElement("node-module");
  }

  public writeGraphNodeData(data: ConnectionData, nodeNode: Element) {
    for (var key in data.getData()) {
      var valueNode : Element = this.mDocument.createElement("value");
      valueNode.setAttribute("key", key);
      valueNode.textContent = data.getData()[key];
      
      nodeNode.appendChild(valueNode);
    }
}

  public writeMetadata(metadata: Metadata, parentNode: Element) {
    var metadataNode: Element = this.mDocument.createElement("metadata");
    var count: number = 0;
    for (var ns of metadata.getNamespaces()) {
      var data: Element = this.mDocument.createElement("data");
      data.setAttribute("namespace", ns);
      this.writeGraphNodeData(metadata.getMetadata(ns), data);
      metadataNode.appendChild(data);
      count++;
    }
    if (count > 0) {
	parentNode.appendChild(metadataNode);
    }
}

  
  public writePackage() {
    var packageEl: Element = this.mDocument.createElement("package");
    packageEl.setAttribute("id", this.mNodeModule.getPackage());
    packageEl.setAttribute("version", "1");
    packageEl.setAttribute("description", this.mNodeModule.getDescription());
    this.mRoot.appendChild(packageEl);

    this.writeMetadata(this.mNodeModule.getMetadata(), this.mRoot);
  }
  public writeScopes() {
  }
  public writeDataTypes() {
  }
  public writeNodeTypes() {
  }
  public writeGraphs() {
  }
  
  public write() : boolean {
    
    this.mRoot.setAttribute("xmlns", SerializerXML.NODEJS_XML_NAMESPACE);

    this.writePackage();
    this.writeScopes();
    this.writeDataTypes();
    this.writeNodeTypes();
    this.writeGraphs();

    this.mDocument.appendChild(this.mRoot);
    
    return true;
  }

}
