
export class DomUtils {

  private static countNewlines(str: string) : number {
    var newlines: number = 0;
    for (var i = 0; i < str.length; i++) {
      if (str[i] == '\n') {
	newlines = newlines + 1;
      }
    }
    return newlines;
  }
  
  // Line numbers are approximate
  // because we can't actually get an accurate
  // line number for element nodes if they
  // span more than one line.
  private static countLinesUntil(parent: Node, target: Element, currentCount: number) : number {
    for (var child of parent.childNodes) {
      if (child.nodeType == DomUtils.NodeType.ELEMENT_NODE) {
	var childElement: Element = child as Element;
	if (target == childElement) {
	  return -currentCount;
	}
	else {
	  var newCount = DomUtils.countLinesUntil(child, target, currentCount);
	  if (newCount < 0) {
	    return newCount;
	  }
	  currentCount = newCount;
	}
      }
      else if (child.nodeType == DomUtils.NodeType.TEXT_NODE) {
	var childText: Text = child as Text;
	currentCount = currentCount + DomUtils.countNewlines(childText.data);
      }
      else if (child.nodeType == DomUtils.NodeType.COMMENT_NODE) {
	var childComment: Comment = child as Comment;
	currentCount = currentCount + DomUtils.countNewlines(childComment.data);
      }
      else {
	console.log("Unhandled node type " + child.nodeType);
	console.log(child);
      }
    }
    return currentCount;
  }
  public static getElementLineno(parent: any, target: Element) : number {
    return -DomUtils.countLinesUntil(parent as Node, target, 1) + 1;
  }

  public static getChildrenByTagName(parent: ChildNode, tagName: string) : Element[] {
    var children: Element[] = [];
    for (var child of parent.childNodes) {
      if (child.nodeType == DomUtils.NodeType.ELEMENT_NODE) {
	var childElement: Element = child as Element;
	if (childElement.tagName == tagName) {
	  children.push(childElement);
	}
      }
    }
    return children;
  }
}

export namespace DomUtils {
  /**
   * This enum represents different types of DOM
   * elements in an XML document.
   */
  export enum NodeType {
    ELEMENT_NODE = 1,
    ATTRIBUTE_NODE = 2,
    TEXT_NODE = 3,
    CDATA_SECTION_NODE = 4,
    PROCESSING_INSTRUCTION_NODE = 7,
    COMMENT_NODE = 8,
    DOCUMENT_NODE = 9,
    DOCUMENT_TYPE_NODE = 10,
    DOCUMENT_FRAGMENT_NODE = 11
  };
}
