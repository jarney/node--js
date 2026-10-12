
import { NodeModule } from "../NodeModule.ts";
import type { ModuleLoader } from "../ModuleLoader.ts";
import type { SerializerErrorReporter } from "../SerializerError.ts";
import { SerializerXML } from "./SerializerXML.ts";

export class ModuleLoaderNodeJSPath implements ModuleLoader {
  mPath: string[];
  mLoadedModules: { [ key: string ] : NodeModule};
  
  constructor() {
    this.mPath = [];
    this.mLoadedModules = {};
  }
  public addPath(path: string) {
    this.mPath.push(path);
  }
  public getPath() : string[] {
    return this.mPath;
  }
  
  // TODO: Load module from HTTP via browser fetch, whatever.
  public async loadModule(
    aFullyQualifiedModuleName: string,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined> {

    var newModule: NodeModule | undefined;
    
    for (var pathIndex in this.mPath) {
      var pathEl: string = this.mPath[pathIndex]!;
      const response = await fetch(aFullyQualifiedModuleName + ".xml");
      console.log(response);
      var xmlString: string;
      xmlString = await response.text();
      if (!response.ok) {
	continue;
      }

      var serializer: SerializerXML = new SerializerXML();
      newModule = this.newModule(aFullyQualifiedModuleName);
      var success: boolean = serializer.read(xmlString, newModule, reporter);
      if (success) break;

      newModule = undefined;
      
    }
    
    console.log("This is really a thing now, isn't it?");
    
    return newModule;
  }
  
  public newModule(aPackageName: string) : NodeModule {
    if (aPackageName in this.mLoadedModules) {
      return this.mLoadedModules[aPackageName]!;
    }
    var nodeModule = new NodeModule(this);
    this.mLoadedModules[aPackageName] = nodeModule;
    return nodeModule;
  }
}

