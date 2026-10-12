
export class SerializerXML {
  public read(
    xmlString: string,
    nodeModule: NodeModule,
    reporter: SerializerErrorReporter
  ) : boolean {
    console.log("Fetched HTTP data");
    console.log(xmlString);
    const parser = new DOMParser();
    const doc1 = parser.parseFromString(xmlString, "application/xml");
    console.log(doc1.contentType); // "application/xml"
    console.log(doc1);
    
    return true;
  }
}
