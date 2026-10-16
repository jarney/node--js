import { describe, it, expect } from "vitest";

import { QualifiedIdentifier } from "../src/QualifiedIdentifier";

describe("QualifiedIdentifier", function(){
  it("test_QualifiedIdentifier_general_tests", function(){

    var localName: QualifiedIdentifier = new QualifiedIdentifier("some-local-node-type");
    expect.assert(!localName.isQualified());
    expect.assert(localName.getName() == "some-local-node-type");

    var scopedName: QualifiedIdentifier = new QualifiedIdentifier("org.ensor.foo");
    expect.assert(scopedName.isQualified());
    expect.assert(scopedName.getName() == "foo");
    expect.assert(scopedName.getNamespace() == "org.ensor");

    // Now some edge cases with too few characters
    // to make sense.
    var emptyNamespace: QualifiedIdentifier = new QualifiedIdentifier(".foo");
    expect.assert(emptyNamespace.isQualified());
    expect.assert(emptyNamespace.getName() == "foo");
    expect.assert(emptyNamespace.getNamespace() == "");

    var emptyName: QualifiedIdentifier = new QualifiedIdentifier("org.ensor.");
    expect.assert(emptyName.isQualified());
    expect.assert(emptyName.getName() == "");
    expect.assert(emptyName.getNamespace() == "org.ensor");

    var emptyEverything: QualifiedIdentifier = new QualifiedIdentifier(".");
    expect.assert(emptyEverything.isQualified());
    expect.assert(emptyEverything.getName() == "");
    expect.assert(emptyEverything.getNamespace() == "");

    var noName: QualifiedIdentifier = new QualifiedIdentifier("");
    expect.assert(!noName.isQualified());
    expect.assert(noName.getName() == "");
    expect.assert(noName.getNamespace() == "");
    });
});
