
export class JOS {
  public static domParser: () => DOMParser;
  public static xmlSerializer: () => XMLSerializer;
}

JOS.domParser = function() {
  return new DOMParser();
};

JOS.xmlSerializer = function() {
  return new XMLSerializer();
};
