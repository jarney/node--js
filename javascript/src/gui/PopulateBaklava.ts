import {ModuleLoader} from "../ModuleLoader";
import {NodeModule} from "../NodeModule";
import {DataType} from "../DataType";
import {NodeType} from "../NodeType";
import {NodePort} from "../NodePort";
import {NamedPorts} from "../NamedPorts";

import { defineNode, NodeInterface } from "@baklavajs/core";
import { NodeInterfaceType, BaklavaInterfaceTypes } from "@baklavajs/interface-types";
import { setType } from "@baklavajs/interface-types";

import type {IBaklavaViewModel} from "@baklavajs/renderer-vue";

export class PopulateBaklava {
  mNodeModule: NodeModule;
  mModuleLoader: ModuleLoader;
  mBaklavaView: IBaklavaViewModel;
  mInterfaceTypeMap: { [key: string] : NodeInterfaceType<string> };
  mUINodeType: { [key: string]: any };

  
   // Define all of the data types first.
  constructor(nodeModule: NodeModule, baklavaView: IBaklavaViewModel) {
    this.mNodeModule = nodeModule;
    this.mModuleLoader= nodeModule.getModuleLoader();

    this.mBaklavaView = baklavaView;

    this.mInterfaceTypeMap = {};
    this.mUINodeType = {};
  }

  public populateDataTypes() : void {
    
    const editor = this.mBaklavaView.editor;
  
    const nodeInterfaceTypes = new BaklavaInterfaceTypes(editor, { viewPlugin: this.mBaklavaView });

    for (const ns of this.mModuleLoader.getLoadedModules()) {
      var mod: NodeModule = this.mModuleLoader.getLoadedModule(ns)!;
      var dataTypeMap: { [ key: string ] : DataType } = mod.getDataTypes();
      for (const id in dataTypeMap) {
        var dataType: DataType = dataTypeMap[id]!;
	var interfaceType: NodeInterfaceType<string> = new NodeInterfaceType<string>(dataType.getName());
	
        console.log("Defining DataType " + dataType.getFullyQualifiedId());
	this.mInterfaceTypeMap[dataType.getFullyQualifiedId()] = interfaceType;
	
	nodeInterfaceTypes.addTypes(interfaceType);
      }
    }
  }

  public populateNamedPorts(ports: NamedPorts, prefix: string) : { [key: string] : () => NodeInterface } {
    var portMap: { [key: string] : () => NodeInterface } = {};
    
    for (var i: number = 0; i < ports.getCount(); i++) {
      const id: string = ports.getName(i);
      const port: NodePort = ports.getByIndex(i)!;
      console.log(prefix + id + ":" + port.getDataType());
      var interfaceType: NodeInterfaceType<string> = this.mInterfaceTypeMap[port.getDataType()]!;
      var dt: string = port.getDataType();
      portMap[id] = () => new NodeInterface(id, "").use(setType, interfaceType);
    }
    return portMap;
  }
  
  public populateNodeTypes() {
    for (const ns of this.mModuleLoader.getLoadedModules()) {
      var mod: NodeModule = this.mModuleLoader.getLoadedModule(ns)!;
      console.log("Loaded module " + ns);
      var nodeTypeMap: { [ key: string ] : NodeType } = mod.getNodeTypes();
      for (const id in nodeTypeMap) {
        var nodeType: NodeType = nodeTypeMap[id]!;
	
        console.log("Defining type " + nodeType.getFullyQualifiedId());

	var inputMap: { [key: string] : () => NodeInterface } = {};
	var inputs: NamedPorts = nodeType.getInputs();

	inputMap = this.populateNamedPorts(inputs, "    > ");
	
	var outputMap: { [key: string] : () => NodeInterface } = {};
	var outputs: NamedPorts = nodeType.getOutputs();
	
	outputMap = this.populateNamedPorts(outputs, "    < ");

	this.mUINodeType[nodeType.getFullyQualifiedId()] = defineNode({
	  type: nodeType.getLocalId(),
	  inputs: inputMap,
	  outputs: outputMap,
	});
        console.log("Registering node type " + nodeType.getFullyQualifiedId());
        this.mBaklavaView.editor.registerNodeType(this.mUINodeType[nodeType.getFullyQualifiedId()]);

      }
    }
  }

  
  public run() : void {

    this.populateDataTypes();
    this.populateNodeTypes();
    
    console.log("Populated node interface types");
    console.log(this.mInterfaceTypeMap);
  }
}
