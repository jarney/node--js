import { NodeModule } from "../src/NodeModule";
import type { ModuleLoaderPlugin } from "../src/ModuleLoader";
import {ModuleLoader} from "../src/ModuleLoader";
import type { SerializerErrorReporter } from "../src/SerializerError";
import { SerializerXML } from "../src/xml/SerializerXML";
import fs from "fs";


export class ModuleLoaderNodeJSPath implements ModuleLoaderPlugin {
  mPath: string[];
  
  constructor() {
    this.mPath = [];
    var path: string | undefined = process.env["NODEJS_PATH"]
    var split: string[] = [];
    if (path != undefined) {
      this.mPath = path.split(":");
    }
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
      var filename: string = pathEl + "/" + aFullyQualifiedModuleName + ".xml";

      try {
	var xmlString: string = (fs.readFileSync(filename) as unknown) as string;
      
	var serializer: SerializerXML = SerializerXML.instance();
	newModule = new NodeModule(aModuleLoader, aFullyQualifiedModuleName);
	var success: boolean = await serializer.read(aFullyQualifiedModuleName, xmlString, newModule, reporter);
	break;
      }
      catch (ex) {
	newModule = undefined;
      }
      
    }
    return newModule;
  }
}

