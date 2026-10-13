import {NodeModule} from "./NodeModule.ts";
import type {SerializerErrorReporter} from "./SerializerError.ts";

export interface Serializer {
  read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : boolean;
  
  write(
    nodeModule: NodeModule,
    out: string,
    reporter: SerializerErrorReporter) : boolean;
}

