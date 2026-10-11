
import { NodeModule } from "../NodeModule.ts";
import type { ModuleLoader } from "../ModuleLoader.ts";
import type { SerializerErrorReporter } from "../SerializerError.ts";

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
  public loadModule(
    aFullyQualifiedModuleName: string,
    reporter: SerializerErrorReporter
  ) : NodeModule | undefined {
    return undefined;
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

