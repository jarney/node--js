
import { NodeTypeId, NodeId, EdgeId, PortId} from "./Definitions.ts"
import { NodeGraph } from "./NodeGraph.ts";
import { NodeType } from "./NodeType.ts";
import { NamedPorts } from "./NamedPorts.ts";
import { ConnectionData } from "./ConnectionData.ts";
import { Metadata } from "./Metadata.ts";


export class Node {
    // Data purely about the abstract node
    // that is the same for each instance.  Factor this out
    // to a node-type class.
    mId: NodeId;
	    
    mType: NodeType;
	    
    mGraph: NodeGraph;
	    
    mData: ConnectionData;

    mMetadata: Metadata;

    mOverrideInputs: boolean;
    mOverrideOutputs: boolean;

    // Ports by name
    mInputs: NamedPorts;
    mOutputs: NamedPorts;
	    
    constructor(
		aId: NodeId,
		aType: NodeType,
		aGraph: NodeGraph,
		aData: ConnectionData
		) {
        this.mId = aId;
	this.mType = aType;
	this.mGraph = aGraph;
	this.mData = new ConnectionData();
	this.mData.copy(aData);
    }
    public getId() : NodeId {
        return this.mId;
    }
    public getType() : NodeType {
        return this.mType;
    }
    public getGraph() : NodeGraph {
        return this.mGraph;
    }
    public getData() : ConnectionData {
        return this.mData;
    }
    public getMetadata() : Metadata {
        return this.mMetadata;
    }
    public getInputs() : NamedPorts {
        if (this.mOverrideInputs) {
	    return this.mInputs;
	}
	else {
	    return this.mType.getInputs();
	}
    }
    public getOutputs() : NamedPorts {
        if (this.mOverrideOutputs) {
	    return this.mOutputs;
	}
	else {
	    return this.mType.getOutputs();
	}
    }
    public getOverrideInputs() : NamedPorts {
        return this.mInputs;
    }
    public getOverrideOutputs() : NamedPorts {
        return this.mOutputs;
    }
    public setOverrideInputs(aOverrideInputs: boolean) {
        this.mOverrideInputs = aOverrideInputs;
    }
    public setOverrideOutputs(aOverrideOutputs: boolean) : boolean {
        this.mOverrideOutputs = aOverrideOutputs;
    }
    public hasOverrideInputs() : boolean {
        return this.mOverrideInputs;
    }
    public hasOverrideOutputs() : boolean {
        return this.mOverrideOutputs;
    }
}
