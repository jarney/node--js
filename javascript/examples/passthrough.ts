#!/usr/bin/env node

import {NodeModule} from "../src/NodeModule";
import {ModuleLoader} from "../src/ModuleLoader";
import {ModuleLoaderNodeJSPath} from "../src/xml/ModuleLoaderNodeJSPath";
import type {SerializerErrorReporter} from "../src/SerializerError";
import {SerializerErrorReporterStream } from "../src/SerializerError";
    
class Foo {
  constructor() {
    console.log("Constructor");
  }
  doit(st: string) : void {
      console.log("And another thing");
      console.log(st);
  }
}

var foo: Foo = new Foo();
foo.doit("xx");

console.log("Hello world");

var moduleLoader: ModuleLoader = new ModuleLoader();
var pathLoader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();
pathLoader.addPath("../test-data/xml/Serializer");
moduleLoader.setPlugin(pathLoader);

var nodeModule: NodeModule = moduleLoader.newModule("Something wild");

console.log("nodeModule package " + nodeModule.getPackage());

var err: SerializerErrorReporter = new SerializerErrorReporterStream();

moduleLoader.loadModule("org.ensor.nodejs.openscad", err);
