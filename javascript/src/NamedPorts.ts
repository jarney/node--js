
import { NodePort } from "./NodePort.ts";

export class NamedPorts {
    mPortsByName: { [ key: string]: NodePort };
    mPorts: NodePort[];
    mPortNames: string[];
       
    constructor() {
        this.mPortsByName = {};
	this.mPorts = [];
	this.mPortNames = [];
    }
    public addPort(name: string, port: NodePort) {
        if (name in this.mPortsByName) {
            return false;
	}
        this.mPortsByName[name] = port;
	this.mPorts.push(port);
	this.mPortNames.push(name);
	return true;
    }
    public getByName(name: string) : NodePort | undefined {
        return this.mPortsByName[name];
    }
    public getByIndex(index: number) : NodePort | undefined {
        return this.mPorts[index];
    }
    public hasPort(name: string) : boolean {
        return name in this.mPortsByName;
    }
    public getName(index: number) : string  {
        if (index >= 0 && index < this.mPortNames.length) {
            return this.mPortNames[index]!;
        }
	return "";
    }
    public getPortIndex(name: string): number {
        var i = 0;
        for (i = 0; i < this.mPortNames.length; i++) {
            if (name == this.mPortNames[i]) {
                return i;
            }
        }
        return -1;
    }
    public getCount() : number {
        return this.mPorts.length;
    }
}
