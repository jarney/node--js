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
var err: SerializerErrorReporter = new SerializerErrorReporterStream();

console.log(process.argv);
if (process.argv.length < 3) {
    console.log("Usage: passthrough package");
    process.exit(1);
}
var packageName: string = process.argv[2];
console.log(process.env["NODEJS_PATH"]);
var path: string | undefined = process.env["NODEJS_PATH"]
var split: string[] = [];
if (path != undefined) {
	split = path.split(":");
}
console.log(split);
//process.exit(1);

moduleLoader.loadModule(packageName, err).then((nodeModule: NodeModule | undefined) => {
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
});
