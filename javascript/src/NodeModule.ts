import {DataType} from "./DataType"
import {NodeType} from "./NodeType"
import {Metadata} from "./Metadata";
import {NodeGraph} from "./NodeGraph";
import {QualifiedIdentifier} from "./QualifiedIdentifier";
import type {ModuleLoader} from "./ModuleLoader";

export class NodeModule {
  mPackage: string = "";
  mDescription: string = ""
  mModuleLoader: ModuleLoader;
  mDataTypes: { [key: string]: DataType } = {};
  mNodeTypes: { [key: string]: NodeType } = {};
  mGraphs: { [ key: string ] : NodeGraph } = {};
  mMetadata: Metadata;
  mImportedModules: { [key: string]: NodeModule } = {};
  
  constructor(
    aModuleLoader: ModuleLoader,
    aFullyQualifiedModuleName: string = ""
  ) {
    this.mPackage = aFullyQualifiedModuleName;
    this.mDescription = aFullyQualifiedModuleName;
    this.mModuleLoader = aModuleLoader;
    this.mDataTypes = {};
    this.mNodeTypes = {};
    this.mGraphs = {};
    this.mMetadata = new Metadata();
    this.mImportedModules = {};
  }
  public setPackage(aPackage: string) {
    this.mPackage = aPackage;
  }
  public getPackage() : string {
    return this.mPackage;
  }
  public setDescription(aDescription: string) {
    this.mDescription = aDescription;
  }
  public getDescription() : string {
    return this.mDescription;
  }
  public getModuleLoader() : ModuleLoader {
    return this.mModuleLoader;
  }
  public addDataType(aDataType: DataType) {
    if (aDataType.getLocalId() in this.mDataTypes) return;
    this.mDataTypes[aDataType.getLocalId()] = aDataType;
  }
  public removeDataType(aId: string) {
    delete this.mDataTypes[aId];
  }
  public removeNodeType(aId: string) : void {
    delete this.mNodeTypes[aId];
  }
  public getDataTypes() : { [key: string]: DataType } {
    return this.mDataTypes;
  }
  public hasDataType(id: string) : boolean {
    var qualifiedName: QualifiedIdentifier = new QualifiedIdentifier(id);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != this.mPackage) {
	// Search the scopes.
      var searchedModule: NodeModule | undefined = this.getImportModule(qualifiedName.getNamespace());
	if (searchedModule == undefined) return false;
	return searchedModule.hasDataType(qualifiedName.getName());
    }
    else {
      return (qualifiedName.getName() in this.mDataTypes);
    }
  }
  public getDataType(id: string) : DataType | undefined {
    var qualifiedName: QualifiedIdentifier = new QualifiedIdentifier(id);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != this.mPackage) {
	// Search the scopes.
      var searchedModule: NodeModule | undefined = this.getImportModule(qualifiedName.getNamespace());
      if (searchedModule == undefined) return undefined;
      return searchedModule.getDataType(qualifiedName.getName());
    }
    else {
      return this.mDataTypes[qualifiedName.getName()];
    }
  }
  public addNodeType(nodeType: NodeType) {
    this.mNodeTypes[nodeType.getId()] = nodeType;
  }
  public getNodeTypes() : { [key: string]: NodeType } {
    return this.mNodeTypes;
  }
  public hasNodeType(id: string) : boolean {
    var qualifiedName: QualifiedIdentifier = new QualifiedIdentifier(id);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != this.mPackage) {
	// Search the scopes.
      var searchedModule: NodeModule | undefined = this.getImportModule(qualifiedName.getNamespace());
	if (searchedModule == undefined) return false;
	return searchedModule.hasNodeType(qualifiedName.getName());
    }
    else {
      return (qualifiedName.getName() in this.mNodeTypes);
    }
  }
  public getNodeType(id: string) : NodeType | undefined {
    var qualifiedName: QualifiedIdentifier = new QualifiedIdentifier(id);
    if (qualifiedName.isQualified() && qualifiedName.getNamespace() != this.mPackage) {
	// Search the scopes.
      var searchedModule: NodeModule | undefined = this.getImportModule(qualifiedName.getNamespace());
      if (searchedModule == undefined) return undefined;
      return searchedModule.getNodeType(qualifiedName.getName());
    }
    else {
      return this.mNodeTypes[qualifiedName.getName()];
    }
  }
  public addGraph(id: string) : NodeGraph | undefined {
    if (id in this.mGraphs) {
      return undefined;
    }
    var g = new NodeGraph(this);
    this.mGraphs[id] = g;
    return g;
  }
  public hasGraph(id: string) : boolean {
    return (id in this.mGraphs);
  }
  public getGraph(id: string) : NodeGraph | undefined {
    return this.mGraphs[id];
  }
  public getGraphs() : { [ key: string ] : NodeGraph } {
    return this.mGraphs;
  }
  public getMetadata() : Metadata {
    return this.mMetadata;
  }
  public addImportModule(nodeModule: NodeModule) : void {
    this.mImportedModules[nodeModule.getPackage()] = nodeModule;
  }
  public hasImportModule(packageId: string) : boolean {
    return packageId in this.mImportedModules;
  }
  public getImportModule(packageId: string) : NodeModule | undefined {
    return this.mImportedModules[packageId];
  }
  public getImportModuleList() : string[] {
    var retList: string[] = [];
    for (var packageId in this.mImportedModules) {
      retList.push(packageId);
    }
    return retList;
  }
}
