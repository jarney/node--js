import type { SerializerErrorReporter } from "./SerializerError";
import { SerializerErrorCode } from "./SerializerError";
import { NodeModule } from "./NodeModule";

export interface ModuleLoaderPlugin {
  loadModule(
    aFullyQualifiedModuleName: string,
    aModuleLoader: ModuleLoader,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined>;
  
}


class ModuleLoaderPluginNil implements ModuleLoaderPlugin {
  constructor() {
  }
  public loadModule(
    aFullyQualifiedModuleName: string,
    aModuleLoader: ModuleLoader,
    reporter: SerializerErrorReporter
  ): Promise<NodeModule | undefined> {
    reporter.reportError(
	SerializerErrorCode.NO_LOADER_DEFINED,
	0,
	aFullyQualifiedModuleName,
	"No module loader was defined, so we cannot load modules."
    );
    return new Promise<NodeModule | undefined>((resolve, reject) => {
      resolve(undefined);
    });
  }
}
		
export class ModuleLoader {
  mPlugin: ModuleLoaderPlugin;
  mLoadedModules: { [key: string] : NodeModule | undefined };
  constructor() {
    this.mPlugin = new ModuleLoaderPluginNil();
    this.mLoadedModules = {};
  }
  public setPlugin(plugin: ModuleLoaderPlugin): void {
    this.mPlugin = plugin;
  }

  public async loadModule(
    aFullyQualifiedModuleName: string,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined> {

    // First, check if we already have it:
    if (aFullyQualifiedModuleName in this.mLoadedModules) {
      return new Promise<NodeModule | undefined>((resolve, reject) => {
	resolve(this.mLoadedModules[aFullyQualifiedModuleName]);
      });
    }
    
    var nodeModule = await this.mPlugin.loadModule(aFullyQualifiedModuleName, this, reporter);
    this.mLoadedModules[aFullyQualifiedModuleName] = nodeModule;
    return nodeModule;
  }

  public newModule(aFullyQualifiedModuleName: string) : NodeModule {
    if (aFullyQualifiedModuleName in this.mLoadedModules) {
      return this.mLoadedModules[aFullyQualifiedModuleName]!;
    }
    var nodeModule: NodeModule = new NodeModule(this, aFullyQualifiedModuleName);
    this.mLoadedModules[aFullyQualifiedModuleName] = nodeModule;
    return nodeModule;
  }

  public getLoadedModules() : string[] {
    var loadedNamespaces: string[] = [];

    for (var ns in this.mLoadedModules) {
	loadedNamespaces.push(ns);
    }
    
    return loadedNamespaces;
  }

  public hasModule(aPackageNamespace: string) : boolean {
    return aPackageNamespace in this.mLoadedModules;
  }

  public getLoadedModule(aPackageNamespace: string) : NodeModule | undefined {
    return this.mLoadedModules[aPackageNamespace];
  }
  
}

