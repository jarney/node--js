import type {NodeId} from "../Definitions";
import type {Serializer} from "../Serializer";
import type {SerializerErrorReporter} from "../SerializerError";
import {NodeModule} from "../NodeModule";
import {SerializerXMLReader} from "./SerializerXMLReader";
import {SerializerXMLWriter} from "./SerializerXMLWriter";
import {JOS} from "../jos";
//import {JSDOM} from "jsdom";
//import {DOMParser} from "@types/dom-parser";

export class SerializerXML implements Serializer {
  public constructor() {
  }
  private static _instance: SerializerXML = new SerializerXML();
  public static NODEJS_XML_NAMESPACE: string = "http://jarney.github.io/nodejs/schema";

  public static instance() {
    return SerializerXML._instance;
  }
  
  public async read(
    aFilename: string,
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : Promise<boolean> {
    
    //const parser = new DOMParser();
    const parser = JOS.domParser();
    const doc = parser.parseFromString(xmlString, "application/xml");

    const serializer: SerializerXMLReader = new SerializerXMLReader(aFilename, doc, nodeModule, reporter);
    return serializer.read();
  }
  public write(
    nodeModule: NodeModule,
    outstream: WritableStream,
    reporter: SerializerErrorReporter) : boolean {

    //outstream.getWriter().write("----- this is lit");

    const parser = JOS.domParser();
    const stubdoc: Document = parser.parseFromString("<?xml version=\"1.0\"><root></root>", "application/xml");

    var xmlDoc: Document = stubdoc.implementation.createDocument("", "", null);

    const writer: SerializerXMLWriter = new SerializerXMLWriter("filename", xmlDoc, nodeModule, reporter);
    writer.write();

    
    const serializer: XMLSerializer = JOS.xmlSerializer();
    serializer.setFeature("http://xmlpull.org/v1/doc/features.html#indent-output", true);
    const xmlStr: string = serializer.serializeToString(xmlDoc);
    outstream.getWriter().write(xmlStr);
    
    return false;
  }
}

