import { describe, it, expect } from "vitest";

import { Edge } from "../src/Edge";

describe("Edge", function(){
    it("test_Edge_equality", function() {
        var conn0 = new Edge("n0", "p0", "n1", "p1");
	
        // Only one way that they can be the same.
	var conn1 = new Edge("n0", "p0", "n1", "p1");
	expect.assert(Edge.equals(conn0, conn1));

        // Lots of different ways it can be different.
	var conn2 = new Edge("n0", "p1", "n1", "p1");
	var conn3 = new Edge("n1", "p0", "n1", "p1");
	var conn4 = new Edge("n0", "p0", "n2", "p1");
	var conn5 = new Edge("n0", "p0", "n1", "p2");

	expect.assert(!Edge.equals(conn0, conn2));
	expect.assert(!Edge.equals(conn0, conn3));
	expect.assert(!Edge.equals(conn0, conn4));
	expect.assert(!Edge.equals(conn0, conn5));
    });

    it("test_Edge_id", function() {
        var conn0 = new Edge("n0", "p0", "n1", "p1");
	expect.assert(conn0.getId() == "n0-p0|n1-p1");
    });
});
