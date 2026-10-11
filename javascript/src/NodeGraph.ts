import {NodeModule} from "./NodeModule.ts";

export class NodeGraph {
  mModule: NodeModule;
  
  constructor(aModule: NodeModule) {
    this.mModule = aModule;
  }
}
