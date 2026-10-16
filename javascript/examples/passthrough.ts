#!/usr/bin/env node

import {NodeModule} from "../src/NodeModule";
import {ModuleLoader} from "../src/ModuleLoader";
import {ModuleLoaderNodeJSPath} from "../src/xml/ModuleLoaderNodeJSPath";
import type {SerializerErrorReporter} from "../src/SerializerError";
import {SerializerErrorReporterStream } from "../src/SerializerError";
import {Serializer} from "../src/Serializer";
import {SerializerXML} from "../src/xml/SerializerXML";
import {JOS} from "../src/jos";
import {JSDOM} from "jsdom";

var x = new JSDOM();
var w = x.window;

JOS.domParser = function() {
    return new w.DOMParser();
};
JOS.xmlSerializer = function() {
    return new w.XMLSerializer();
};

var moduleLoader: ModuleLoader = new ModuleLoader();
var pathLoader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
pathLoader.addPath("../../test-data/xml/Serializer");
moduleLoader.setPlugin(pathLoader);

var nodeModule: NodeModule = moduleLoader.newModule("Something wild");

console.log("nodeModule package " + nodeModule.getPackage());

var err: SerializerErrorReporter = new SerializerErrorReporterStream();

function finishit(nodeModule: NodeModule | undefined) {
    if (nodeModule == undefined) {
	console.log("Error: could nod load module");
	return;
    }
   
    var outstream: WritableStream = new WritableStream({
	write(chunk) {
	    console.log(chunk);
	}
    });
    
    var ser: Serializer = new SerializerXML();
    ser.write(nodeModule!, outstream, err);
}
moduleLoader.loadModule("org.ensor.nodejs.openscad", err).then((nodeModule2: NodeModule | undefined) => {
    console.log(nodeModule2);
    finishit(nodeModule2);
});
