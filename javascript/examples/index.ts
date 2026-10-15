#!/usr/bin/env node

import {NodeModule} from "../src/NodeModule"
import {ModuleLoaderNodeJSPath} from "../src/xml/ModuleLoaderNodeJSPath"

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

var moduleLoader: ModuleLoaderNodeJSPath = new ModuleLoaderNodeJSPath();

var nodeModule: NodeModule = moduleLoader.newModule("Something wild");

console.log("nodeModule package " + nodeModule.getPackage());

