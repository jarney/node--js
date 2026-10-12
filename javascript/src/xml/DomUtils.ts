
function countNewlines(str: string) : number {
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
function countLinesUntil(parent: Node, target: Element, currentCount: number) : number {
  for (var child of parent.childNodes) {
    if (child.nodeType == ELEMENT_NODE) {
      var childElement: Element = child as Element;
      if (target == childElement) {
	return -currentCount;
      }
      else {
	var newCount = countLinesUntil(child, target, currentCount);
	if (newCount < 0) {
	  return newCount;
	}
	currentCount = newCount;
      }
    }
    else if (child.nodeType == TEXT_NODE) {
      var childText: Text = child as Text;
      currentCount = currentCount + countNewlines(childText.data);
    }
    else if (child.nodeType == COMMENT_NODE) {
      var childComment: Comment = child as Comment;
      currentCount = currentCount + countNewlines(childComment.data);
    }
    else {
      console.log("Unhandled node type " + child.nodeType);
      console.log(child);
    }
  }
  return currentCount;
}
function getElementLineno(parent: any, target: Element) : number {
  return -countLinesUntil(parent as Node, target, 1) + 1;
}

const ELEMENT_NODE: number = 1;
const ATTRIBUTE_NODE: number = 2;
const TEXT_NODE: number = 3;
const CDATA_SECTION_NODE: number = 4;
const PROCESSING_INSTRUCTION_NODE: number = 7;
const COMMENT_NODE: number = 8;
const DOCUMENT_NODE: number = 9;
const DOCUMENT_TYPE_NODE: number = 10;
const DOCUMENT_FRAGMENT_NODE: number = 11;

export {
  getElementLineno,
  ELEMENT_NODE,
  ATTRIBUTE_NODE,
  TEXT_NODE,
  CDATA_SECTION_NODE,
  PROCESSING_INSTRUCTION_NODE,
  COMMENT_NODE,
  DOCUMENT_NODE,
  DOCUMENT_TYPE_NODE,
  DOCUMENT_FRAGMENT_NODE
};


