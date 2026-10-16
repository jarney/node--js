import {NodeModule} from "./NodeModule";
import type {SerializerErrorReporter} from "./SerializerError";

export interface Serializer {
  read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : Promise<boolean>;
  
  write(
    nodeModule: NodeModule,
    outstream: WritableStream,
    reporter: SerializerErrorReporter) : boolean;
}

