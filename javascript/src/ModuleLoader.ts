import type { SerializerErrorReporter } from "./SerializerError";
import { NodeModule } from "./NodeModule";

export interface ModuleLoader {
  loadModule(
    aFullyQualifiedModuleName: string,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined>;
  
  newModule(aPackageName: string) : NodeModule;
}

