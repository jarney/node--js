import { ConnectionData } from "./ConnectionData.ts";

/**
 * A port is a part of a node that can exchange data.
 * Ports may play the role of either input or output data
 * in a node and may transmit data along the port.  Each port
 * has a data type and that data type must match between
 * the output of one node and the input of the next along
 * the connection.  A node may decide how it interprets
 * the data carried in the port, but ideally, multiple
 * nodes will agree on an encoding so that they can inter-operate.
 */
export class Metadata {
    mMetadata: {};

    constructor() {
        this.mMetadata = {};
    }
    public getMetadata(aMetadataNamespace: string ) : ConnectionData {
        if (!(aMetadataNamespace in this.mMetadata)) {
            this.mMetadata[aMetadataNamespace] = new ConnectionData();
	}
        return this.mMetadata[aMetadataNamespace];	
    }
    public hasMetadata(aMetadataNamespace: string) : boolean {
        return aMetadataNamespace in this.mMetadata;
    }
    public getNamespaces(): [string] {
        var ret = [];
	for (var n in this.mMetadata) {
	    ret.push(n);
	}
	return ret;
    }
};
