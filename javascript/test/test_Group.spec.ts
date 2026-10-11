import { describe, it, expect } from "vitest"

import { Group } from "../src/Group.ts";

describe("Group", () => {
    it("test_Group_check_basic_stuff", () => {
        var g = new Group();

        // Add node, make sure we can find it.
        g.addNode("foo");
        expect.assert(g.contains("foo"));
        expect(g.getNodes()).toEqual(["foo"]);

        // Remove node, make sure it goes away.
        g.removeNode("foo");
        expect.assert(!g.contains("foo"));

        // Make sure non-existing node isnt there.
        expect.assert(!g.contains("bar"));
        g.removeNode("bar");
        expect.assert(!g.contains("bar"));

        expect(g.getNodes()).toEqual([]);

    });
});
