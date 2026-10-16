import type {NodeId} from "../Definitions";
import type {Serializer} from "../Serializer";
import type {SerializerErrorReporter} from "../SerializerError";
import {NodeModule} from "../NodeModule";
import {SerializerXMLReader} from "./SerializerXMLReader";

export class SerializerXML implements Serializer {
  private constructor() {
  }
  private static _instance: SerializerXML = new SerializerXML();

  public static instance() {
    return SerializerXML._instance;
  }
  
  public async read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : Promise<boolean> {
    
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

