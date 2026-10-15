import {DataType} from "./DataType"
import {NodeType} from "./NodeType"
import {Metadata} from "./Metadata";
import {NodeGraph} from "./NodeGraph";
import type {ModuleLoader} from "./ModuleLoader";

export class NodeModule {
  mPackage: string = "";
  mDescription: string = ""
  mModuleLoader: ModuleLoader;
  mDataTypes: { [key: string]: DataType } = {};
  mNodeTypes: { [key: string]: NodeType } = {};
  mGraphs: { [ key: string ] : NodeGraph } = {};
  mMetadata: Metadata;
  
  constructor(
    aModuleLoader: ModuleLoader
  ) {
    this.mPackage = "";
    this.mDescription = "";
    this.mModuleLoader = aModuleLoader;
    this.mDataTypes = {};
    this.mNodeTypes = {};
    this.mGraphs = {};
    this.mMetadata = new Metadata();
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
    if (aDataType.getId() in this.mDataTypes) return;
    this.mDataTypes[aDataType.getId()] = aDataType;
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
    return (id in this.mDataTypes);
  }
  public getDataType(id: string) : DataType | undefined {
    return this.mDataTypes[id];
  }
  public addNodeType(nodeType: NodeType) {
    this.mNodeTypes[nodeType.getId()] = nodeType;
  }
  public getNodeTypes() : { [key: string]: NodeType } {
    return this.mNodeTypes;
  }
  public hasNodeType(id: string) : boolean {
    return (id in this.mNodeTypes);
  }
  public getNodeType(id: string) : NodeType | undefined {
    return this.mNodeTypes[id];
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
}



