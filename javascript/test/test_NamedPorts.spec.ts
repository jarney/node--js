import { describe, it, expect } from "vitest"

import { NamedPorts } from "../src/NamedPorts";
import { NodePort } from "../src/NodePort";

describe("NamedPorts", () => {
    it("test_NamedPorts_features", () => {
        var DATA_TYPE = "variable";
        var namedPort = new NamedPorts();

        namedPort.addPort("a", new NodePort(DATA_TYPE, "adesc", NodePort.ConnectionPolicy.Multiple));
        namedPort.addPort("b", new NodePort(DATA_TYPE, "bdesc", NodePort.ConnectionPolicy.Multiple));
        namedPort.addPort("c", new NodePort(DATA_TYPE, "cdesc", NodePort.ConnectionPolicy.Multiple));
        namedPort.addPort("d", new NodePort(DATA_TYPE, "ddesc", NodePort.ConnectionPolicy.Multiple));

        expect.assert(namedPort.hasPort("a"));
        expect.assert(namedPort.hasPort("b"));
        expect.assert(namedPort.hasPort("c"));
        expect.assert(namedPort.hasPort("d"));

        expect.assert(!namedPort.hasPort("z"));

        var na = namedPort.getByName("a");
        expect.assert(na != undefined);
        expect.assert(namedPort.getByName("a").getDescription() == "adesc");
        expect.assert(namedPort.getByName("b").getDescription() == "bdesc");
        expect.assert(namedPort.getByName("c").getDescription() == "cdesc");
        expect.assert(namedPort.getByName("d").getDescription() == "ddesc");

        var nnon = namedPort.getByName("z");
        expect.assert(nnon == undefined);

        expect.assert(namedPort.getPortIndex("a") == 0);
        expect.assert(namedPort.getPortIndex("b") == 1);
        expect.assert(namedPort.getPortIndex("c") == 2);
        expect.assert(namedPort.getPortIndex("d") == 3);
        expect.assert(namedPort.getPortIndex("z") == -1);

        expect.assert(namedPort.getCount() == 4);
        expect.assert(namedPort.getByIndex(0).getDescription() == "adesc");
        expect.assert(namedPort.getByIndex(1).getDescription() == "bdesc");
        expect.assert(namedPort.getByIndex(2).getDescription() == "cdesc");
        expect.assert(namedPort.getByIndex(3).getDescription() == "ddesc");
        expect.assert(namedPort.getByIndex(4) == undefined);

        expect.assert(namedPort.getName(0) == "a");
        expect.assert(namedPort.getName(1) == "b");
        expect.assert(namedPort.getName(2) == "c");
        expect.assert(namedPort.getName(3) == "d");
    });
});
