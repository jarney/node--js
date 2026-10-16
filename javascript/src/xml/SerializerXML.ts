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

  private static INDENT_PATTERN = '  ';
  private static formatXml(xml: string, tab: string): string { // tab = optional indent value, default is tab (\t)
    var formatted = '', indent= '';
    tab = tab || SerializerXML.INDENT_PATTERN;
    xml.split(/>\s*</).forEach(function(node) {
      if (node.match( /^\/\w/ )) indent = indent.substring(tab.length); // decrease indent by one 'tab'
      formatted += indent + '<' + node + '>\n';
      if (node.match( /^<?\w[^>]*[^\/]$/ )) indent += tab;              // increase indent
    });
    return formatted.substring(1, formatted.length-3);
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
    const xmlStr: string = serializer.serializeToString(xmlDoc);
    
    outstream.getWriter().write(SerializerXML.formatXml(xmlStr, ''));
    
    return false;
  }
}

