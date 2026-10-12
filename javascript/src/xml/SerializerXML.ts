import type {SerializerErrorReporter} from "../SerializerError.ts";

import {NodeModule} from "../NodeModule.ts";
import {DataType} from "../DataType.ts";
import {NodeType} from "../NodeType.ts";
import {Node} from "../Node.ts";
import {SerializerErrorCode, SerializerErrorReporterNil} from "../SerializerError.ts";
import {DomUtils} from "./DomUtils.ts";

export class SerializerXML {
  mRoot: any;
  mFilename: string;
  mReporter: SerializerErrorReporter;
  
  constructor() {
    this.mRoot = undefined;
    this.mFilename = "unknown";
    this.mReporter = new SerializerErrorReporterNil();
  }

  public parseError(error_code: number, element: Element, message: string): boolean {
    var lineno: number = DomUtils.getElementLineno(this.mRoot, element);
    this.mReporter.reportError(error_code, lineno, this.mFilename, message);
    return false;
  }
  public parsePackage(packageElement: Element, nodeModule: NodeModule) : boolean {
    var id: string | null = packageElement.getAttribute("id");
    if (id == null) {
      return this.parseError(SerializerErrorCode.PACKAGE_ID_MISSING, packageElement, "Package element is missing the 'id' element.");
    }
    if (id != this.mFilename) {
      return this.parseError(SerializerErrorCode.PACKAGE_ID_INCONSISTENT, packageElement, "Package id should be the same as the file it was loaded from.");
    }
    nodeModule.setPackage(id!);

    // The package description is optional.
    var description: string | null = packageElement.getAttribute("description");
    if (description != null) {
      nodeModule.setDescription(description!);
    }

    return true;
  }
  
  
  private parseDataType(dataTypeNode: Element, nodeModule: NodeModule) : boolean {
    var id: string | null = dataTypeNode.getAttribute("id");
    var name: string | null = dataTypeNode.getAttribute("name");
    if (id == null || name == null) {
      return this.parseError(SerializerErrorCode.BAD_DATA_TYPE, dataTypeNode, "Missing id or name for data type");
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
    const parser = new DOMParser();
    const doc = parser.parseFromString(xmlString, "application/xml");

    this.mRoot = doc;
    this.mFilename = aFilename;
    this.mReporter = reporter;

    const root = doc.documentElement;

    var packageCount = 0;
    for (var packageElement of DomUtils.getChildrenByTagName(root, "package")) {
      if (!this.parsePackage(packageElement, nodeModule)) return false;
      if (packageCount > 1) {
	return this.parseError(SerializerErrorCode.PACKAGE_DUPLICATE, packageElement,
	  "Too many package declarations.  There should only be a single package declaration in a module.");
      }
      packageCount++;
    }
    if (packageCount < 1) {
      return this.parseError(SerializerErrorCode.PACKAGE_MISSING, root,
	"Package declaration missing.  Module must contain a <package id='...'/> element.");
    }

    // Data types need to be read before node types
    // even if they appear later in the file.
    for (var dataTypesNode of DomUtils.getChildrenByTagName(root, "data-types")) {
      for (var dataTypeNode of DomUtils.getChildrenByTagName(dataTypesNode, "data-type")) {
	if (!this.parseDataType(dataTypeNode, nodeModule)) return false;
      }
    }
    return true;
  }
}

