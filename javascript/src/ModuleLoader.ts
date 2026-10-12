import type { SerializerErrorReporter } from "./SerializerError.ts";
import { NodeModule } from "./NodeModule.ts";

export interface ModuleLoader {
  loadModule(
    aFullyQualifiedModuleName: string,
    reporter: SerializerErrorReporter
  ) : Promise<NodeModule | undefined>;
  
  newModule(aPackageName: string) : NodeModule;
}

