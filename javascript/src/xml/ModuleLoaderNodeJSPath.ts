
import { NodeModule } from "../NodeModule";
import type { ModuleLoaderPlugin } from "../ModuleLoader";
import {ModuleLoader} from "../ModuleLoader";
import type { SerializerErrorReporter } from "../SerializerError";
import { SerializerXML } from "./SerializerXML";

export class ModuleLoaderNodeJSPath implements ModuleLoaderPlugin {
  mPath: string[];
  
  constructor() {
    this.mPath = [];
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
    aModuleLoader: ModuleLoader,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined> {

    var newModule: NodeModule | undefined;
    
    for (var pathIndex in this.mPath) {
      var pathEl: string = this.mPath[pathIndex]!;
      var filename: string = aFullyQualifiedModuleName + ".xml";
      const response = await fetch(filename);
      console.log(response);
      var xmlString: string;
      xmlString = await response.text();
      if (!response.ok) {
	continue;
      }

      var serializer: SerializerXML = SerializerXML.instance();
      newModule = new NodeModule(aModuleLoader, aFullyQualifiedModuleName);
      var success: boolean = await serializer.read(aFullyQualifiedModuleName, xmlString, newModule, reporter);
      if (success) break;

      newModule = undefined;
      
    }
    
    console.log("This is really a thing now, isn't it?");
    
    return newModule;
  }
}

