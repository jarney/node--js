import type {NodeId} from "../Definitions.ts";
import type {Serializer} from "../Serializer.ts";
import type {SerializerErrorReporter} from "../SerializerError.ts";
import {NodeModule} from "../NodeModule.ts";
import {SerializerXMLReader} from "./SerializerXMLReader.ts";

export class SerializerXML implements Serializer {
  private constructor() {
  }
  private static _instance: SerializerXML = new SerializerXML();

  public static instance() {
    return SerializerXML._instance;
  }
  
  public read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : boolean {
    
    const parser = new DOMParser();
    const doc = parser.parseFromString(xmlString, "application/xml");

    const serializer: SerializerXMLReader = new SerializerXMLReader(aFilename, doc, nodeModule, reporter);
    return serializer.read();
  }
  public write(
    nodeModule: NodeModule,
    out: string,
    reporter: SerializerErrorReporter) : boolean {
    // TODO: Not yet implemented.
    return false;
  }
}

