import { describe, it, expect } from "vitest"

import { ConnectionData } from "../src/ConnectionData.ts";

describe("ConnectionData", () => {
    it("test_ConnectionData_constructor", () => {
        var cd = new ConnectionData();
    });

    it("test_ConnectionData_default_values", () => {
        var cd = new ConnectionData();

	expect.assert(!cd.hasValue("unknown-value"));
    
        // If we specify a default, check that it's honored.
	expect.assert(cd.getValue("unknown-value", "some-default") == "some-default");

	expect.assert(cd.getValue("unknown-value") == "");
    });

    it("test_ConnectionData_actual_values", () => {
        var cd = new ConnectionData();

        // If we actually have a value, make sure it works.
        cd.setValue("well-known-value", "actual value");
    
        expect.assert(cd.hasValue("well-known-value"));
        expect.assert(cd.getValue("well-known-value", "other-default") == "actual value");
    });

    it("test_ConnectionData_copy", () => {
        var cd = new ConnectionData();
        // If we actually have a value, make sure it works.
        cd.setValue("well-known-value", "actual value");

        // Check that whatever we do to one,
        // is done to the other.
        var cdOther = new ConnectionData();
	cdOther.copy(cd);
        cdOther.setValue("only-other-value", "blue");

        expect.assert(cdOther.hasValue("well-known-value"));
        expect.assert(cdOther.getValue("well-known-value", "other-default") == "actual value");

        // Check that the new value ONLY shows up on the new list.
        expect.assert(!cd.hasValue("only-other-value"));
        expect.assert(cdOther.hasValue("only-other-value"));
    });

    it("test_ConnectionData_data", () => {
        var cd = new ConnectionData();
        cd.setValue("well-known-value", "actual value");
    
        var d = cd.getData();

        expect.assert("well-known-value" in d);
        expect.assert(d["well-known-value"]  == "actual value");
    });

    it("test_ConnectionData_append", () => {
        var cd = new ConnectionData();
        cd.setValue("well-known-value", "value-1");
        cd.appendValue("well-known-value", "value-2");
        expect.assert(cd.getValue("well-known-value") == "value-1value-2");
    });
});