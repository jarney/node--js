
//const DOMParser = require("jsdom/lib/generated/idl/DOMParser");

//const {installInterfaces} = require("jsdom/lib/jsdom/living/interfaces");
//var window = {
//    Object: Object
//};

//installInterfaces(window, ["Window"]);
//installInterfaces(Object, ["this"]);

const jsdom = require("jsdom");
var x = new jsdom.JSDOM();

var w = x.window;
const DOMParser = w.DOMParser;
//console.log(DOMParser);


//new w.DOMParser();
var d = new DOMParser();

